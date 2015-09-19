#include "LauncherTool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>
#include <limits.h>

#define MAX_LEN 512
#define JVM_SO "libjvm.so"
#define I386 "i386"
#define AMD64 "amd64"
#define JVM_CLIENT_SO_PATH "client"
#define JVM_SERVER_SO_PATH "server"

char* find_jvm_so(const char* java_home_path);
void* load_jvm_so(const char* so_path);
char* get_arch();

int load_jvm(const char* class_path, JavaVM** java_vm, JNIEnv** jni_env)
{
  char* java_home = getenv(JAVA_HOME);
  char java_home_path[MAX_LEN];
  char* jvm_so_path = NULL;
  void* libjvm = NULL;
  int find_flag = JNI_FALSE;
  int len;
  if(java_home && (len = strlen(java_home)) > 0)
  {
    printf("JAVA_HOME=%s\n", java_home);
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
						if((libjvm = load_jvm_so(jvm_so_path)) != NULL)
						{
              find_flag = JNI_TRUE;
              break;
            }
            free(jvm_so_path);
            jvm_so_path = NULL;
					}
				}
			}
		}
  }

  if(!find_flag)
    java_home = getenv(PATH);
  if(!find_flag && java_home && (len = strlen(java_home)) > 0)
  {
    printf("PATH=%s\n", java_home);
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
						if((libjvm = load_jvm_so(jvm_so_path)) != NULL)
						{
              find_flag = JNI_TRUE;
              break;
            }
            free(jvm_so_path);
            jvm_so_path = NULL;
					}
				}
			}
		}
  }

  if(!find_flag || jvm_so_path == NULL || libjvm == NULL)
  {
    fprintf(stderr, "Can't find %s(%s version).\n", JVM_SO, get_arch());
    return JNI_FALSE;
  }

  printf("%s found:%s\n", JVM_SO, jvm_so_path);
  free(jvm_so_path);

  jint(JNICALL *jvm_create_proc)(JavaVM**, void**, void*) = NULL;
  jvm_create_proc = dlsym(libjvm, "JNI_CreateJavaVM");
  if(jvm_create_proc == NULL)
  {
    fprintf(stderr, "The entry of the function \"JNI_CreateJavaVM\" can not be found in %s.\n", JVM_SO);
      return JNI_FALSE;
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
		return JNI_FALSE;
  }
	return JNI_TRUE;
}

char* find_jvm_so(const char* java_home_path)
{
  char* path = malloc(sizeof(char) * MAX_LEN);
  sprintf(path, "%s%c%s", java_home_path, FILE_SEPARATOR, JVM_SO);
  if(check_file_exist(path))
    return path;

  //java_home/lib/i386(amd64)/client(server)/libjvm.so
  sprintf(path, "%s%clib%c%s%c%s%c%s", java_home_path, FILE_SEPARATOR, FILE_SEPARATOR,
          get_arch(), FILE_SEPARATOR, JVM_CLIENT_SO_PATH, FILE_SEPARATOR, JVM_SO);
  if(check_file_exist(path))
    return path;

  sprintf(path, "%s%clib%c%s%c%s%c%s", java_home_path, FILE_SEPARATOR, FILE_SEPARATOR,
          get_arch(), FILE_SEPARATOR, JVM_SERVER_SO_PATH, FILE_SEPARATOR, JVM_SO);
  if(check_file_exist(path))
    return path;


  //java_home/jre/lib/i386(amd64)/client(server)/libjvm.so
  sprintf(path, "%s%cjre%clib%c%s%c%s%c%s", java_home_path, FILE_SEPARATOR, FILE_SEPARATOR, FILE_SEPARATOR,
          get_arch(), FILE_SEPARATOR, JVM_CLIENT_SO_PATH, FILE_SEPARATOR, JVM_SO);
  if(check_file_exist(path))
    return path;

  sprintf(path, "%s%cjre%clib%c%s%c%s%c%s", java_home_path, FILE_SEPARATOR, FILE_SEPARATOR, FILE_SEPARATOR,
          get_arch(), FILE_SEPARATOR, JVM_SERVER_SO_PATH, FILE_SEPARATOR, JVM_SO);
  if(check_file_exist(path))
    return path;

  free(path);
  return NULL;
}

char* get_arch()
{
  int bits = CHAR_BIT * sizeof(void*);
  if(bits == 32)
    return I386;
  else if(bits == 64)
    return AMD64;
  return I386;
}

void* load_jvm_so(const char* so_path)
{
  void* jvm_so = dlopen(so_path, RTLD_NOW + RTLD_GLOBAL);
  if(jvm_so == NULL)
  {
    fprintf(stderr, "%s\n", dlerror());
    return NULL;
  }
  return jvm_so;
}
