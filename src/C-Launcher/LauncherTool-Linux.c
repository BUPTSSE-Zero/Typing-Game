#include "LauncherTool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>
#include <limits.h>

#define JVM_CLIENT_SO_PATH "client"
#define JVM_SERVER_SO_PATH "server"

typedef jint(JNICALL *JvmCreateFun)(JavaVM**, void**, void*);
char* find_jvm_so(const char* java_home_path);

int load_jvm_from_env_variable(const char* env_variable, const char* class_path, JavaVM** java_vm, JNIEnv** jni_env)
{
  char* java_home = getenv(env_variable);
  char java_home_path[MAX_LEN];
  char* jvm_so_path = NULL;
  int len;
  if(java_home && (len = strlen(java_home)) > 0)
  {
    printf("%s=%s\n",env_variable, java_home);
		int i, pos = 0;
		for (i = 0; i <= len; i++)
		{
			if (java_home[i] == PATH_SEPARATOR || i == len)
			{
				if (sub_str(java_home, pos, i - pos, java_home_path))
				{
					if (java_home_path[strlen(java_home_path) - 1] == FILE_SEPARATOR)
						java_home_path[strlen(java_home_path) - 1] = '\0';
					jvm_so_path = find_jvm_so(java_home_path);
					if (jvm_so_path != NULL)
					{
						if(load_jvm_dll(jvm_so_path, class_path, java_vm, jni_env) == JVM_LOAD_SUCCESS)
						{
              free(jvm_so_path);
              return JNI_TRUE;
						}
            free(jvm_so_path);
            jvm_so_path = NULL;
					}
				}
			}
		}
  }
  return JNI_FALSE;
}

int load_jvm(const char* class_path, JavaVM** java_vm, JNIEnv** jni_env)
{
  if(load_jvm_from_env_variable(JAVA_HOME, class_path, java_vm, jni_env))
    return JNI_TRUE;
  if(load_jvm_from_env_variable(PATH, class_path, java_vm, jni_env))
    return JNI_TRUE;
	return JNI_FALSE;
}

char* find_jvm_so(const char* java_home_path)
{
  char* path = malloc(sizeof(char) * MAX_LEN);

  //java_home/lib/i386(amd64)/client(server)/libjvm.so
  sprintf(path, "%s%clib%c%s%c%s%c%s", java_home_path, FILE_SEPARATOR, FILE_SEPARATOR,
          get_arch(), FILE_SEPARATOR, JVM_CLIENT_SO_PATH, FILE_SEPARATOR, JVM_DLL);
  if(check_file_exist(path))
    return path;

  sprintf(path, "%s%clib%c%s%c%s%c%s", java_home_path, FILE_SEPARATOR, FILE_SEPARATOR,
          get_arch(), FILE_SEPARATOR, JVM_SERVER_SO_PATH, FILE_SEPARATOR, JVM_DLL);
  if(check_file_exist(path))
    return path;


  //java_home/jre/lib/i386(amd64)/client(server)/libjvm.so
  sprintf(path, "%s%cjre%clib%c%s%c%s%c%s", java_home_path, FILE_SEPARATOR, FILE_SEPARATOR, FILE_SEPARATOR,
          get_arch(), FILE_SEPARATOR, JVM_CLIENT_SO_PATH, FILE_SEPARATOR, JVM_DLL);
  if(check_file_exist(path))
    return path;

  sprintf(path, "%s%cjre%clib%c%s%c%s%c%s", java_home_path, FILE_SEPARATOR, FILE_SEPARATOR, FILE_SEPARATOR,
          get_arch(), FILE_SEPARATOR, JVM_SERVER_SO_PATH, FILE_SEPARATOR, JVM_DLL);
  if(check_file_exist(path))
    return path;

  sprintf(path, "%s%c%s", java_home_path, FILE_SEPARATOR, JVM_DLL);
  if(check_file_exist(path))
    return path;

  free(path);
  return NULL;
}

enum JvmLoadErrorType load_jvm_dll(const char* jvm_dll_path, const char* class_path, JavaVM** java_vm, JNIEnv** jni_env)
{
  void* jvm_so = dlopen(jvm_dll_path, RTLD_NOW + RTLD_GLOBAL);
  if(!check_file_exist(jvm_dll_path))
    return JVM_DLL_NOT_FOUND;
  if(jvm_so == NULL)
  {
    fprintf(stderr, "%s\n", dlerror());
    return JVM_DLL_LOAD_FAILED;
  }
  JvmCreateFun jvm_create_proc = dlsym(jvm_so, JVM_CREATE_FUN);
  if(jvm_create_proc == NULL)
  {
    fprintf(stderr, "%s\n", dlerror());
    dlclose(jvm_so);
    return JVM_DLL_ENTRY_NOT_FOUND;
  }

  JavaVMOption vm_options[2];
	JavaVMInitArgs vm_init_args;
	char jvm_class_path[MAX_LEN];
	sprintf(jvm_class_path, "-Djava.class.path=%s", class_path);

	//Init args
	vm_options[0].optionString = jvm_class_path;
	vm_options[1].optionString = "-Dfile.encoding=utf-8";								//set encoding of runtime to UTF-8
	memset(&vm_init_args, 0, sizeof(vm_init_args));
	vm_init_args.version = JNI_VERSION_1_6;
	vm_init_args.nOptions = 2;
	vm_init_args.options = vm_options;

  if (jvm_create_proc(java_vm, (void**)jni_env, (void*)&vm_init_args) != 0)
  {
    fprintf(stderr, "Create JVM failed.\n");
    dlclose(jvm_so);
		return JVM_CREATE_FAILED;
  }
  if((**jni_env)->GetVersion(*jni_env) < JRE_MINIMUM_VERSION_LINUX)
  {
    (**java_vm)->DestroyJavaVM(*java_vm);
    dlclose(jvm_so);
    return JVM_VERSION_ERROR;
  }
  return JVM_LOAD_SUCCESS;
}
