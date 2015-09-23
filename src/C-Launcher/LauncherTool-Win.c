#include "LauncherTool.h"
#include <windows.h>
#include <winreg.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

#define JVM_DLL_CLIENT_PATH "client"
#define JVM_DLL_SERVER_PATH "server"
#define JRE_KEY "Software\\JavaSoft\\Java Runtime Environment"

typedef jint(JNICALL *JvmCreateFun)(JavaVM**, void**, void*);

char* find_jvm_dll(const char* java_home_path);
char* find_public_jre();
int get_string_from_registry(HKEY key, const char* name, char* buf, unsigned int buf_size);

int load_jvm(const char* class_path, JavaVM** java_vm, JNIEnv** jni_env)
{
	char java_home[MAX_LEN];
	char java_home_path[MAX_LEN];
	char* jvm_dll_path = NULL;
	JvmCreateFun jvm_create_proc = NULL;

	if (GetEnvironmentVariable(JAVA_HOME, java_home, sizeof(java_home)) > 0)
	{
		printf("JAVA_HOME=%s\n", java_home);
		int i, len = strlen(java_home), pos = 0;
		for (i = 0; i <= len; i++)
		{
			if (java_home[i] == PATH_SEPARATOR || i == len)
			{
				if (sub_str(java_home, pos, i - pos, java_home_path))
				{
					if (java_home_path[strlen(java_home_path) - 1] == FILE_SEPARATOR)
						java_home_path[strlen(java_home_path) - 1] = '\0';
					jvm_dll_path = find_jvm_dll(java_home_path);
					if (jvm_dll_path != NULL)
					{
						if (load_jvm_dll(jvm_dll_path, class_path, java_vm, jni_env) == JVM_LOAD_SUCCESS)
						{
							free(jvm_dll_path);
							return JNI_TRUE;
						}
						free(jvm_dll_path);
						jvm_dll_path = NULL;
					}
				}
			}
		}
	}

	if (GetEnvironmentVariable(PATH, java_home, sizeof(java_home)) > 0)
	{
		printf("PATH=%s\n", java_home);
		int i, len = strlen(java_home), pos = 0;
		for (i = 0; i <= len; i++)
		{
			if (java_home[i] == PATH_SEPARATOR || i == len)
			{
				if (sub_str(java_home, pos, i - pos, java_home_path))
				{
					if (java_home_path[strlen(java_home_path) - 1] == FILE_SEPARATOR)
						java_home_path[strlen(java_home_path) - 1] = '\0';
					jvm_dll_path = find_jvm_dll(java_home_path);
					if (jvm_dll_path != NULL)
					{
						if (load_jvm_dll(jvm_dll_path, class_path, java_vm, jni_env) == JVM_LOAD_SUCCESS)
						{
							free(jvm_dll_path);
							return JNI_TRUE;
						}
						free(jvm_dll_path);
						jvm_dll_path = NULL;
					}
				}
			}
		}
	}

	char* jre_path = find_public_jre();
	if (jre_path)
	{
		if (jre_path[strlen(jre_path) - 1] == FILE_SEPARATOR)
			jre_path[strlen(jre_path) - 1] = '\0';
		printf("Public JRE path:%s\n", jre_path);
		jvm_dll_path = find_jvm_dll(jre_path);
		free(jre_path);
		if (jvm_dll_path != NULL)
		{
			if (load_jvm_dll(jvm_dll_path, class_path, java_vm, jni_env) == JVM_LOAD_SUCCESS)
			{
				free(jvm_dll_path);
				return JNI_TRUE;
			}
			else
			{
				free(jvm_dll_path);
				jvm_dll_path = NULL;
			}
		}
	}
	return JNI_FALSE;
}

char* find_jvm_dll(const char* java_home_path)
{
	char* path = malloc(sizeof(char) * MAX_LEN);
	char jvm_cfg_path[MAX_LEN];

	//java_home\lib\i386(amd64)\jvm.cfg
	sprintf(jvm_cfg_path, "%s%clib%c%s%c%s", java_home_path, FILE_SEPARATOR, FILE_SEPARATOR, get_arch(), FILE_SEPARATOR, JVM_CFG);
	if (check_file_exist(jvm_cfg_path))
	{
		//java_home\bin\client(server)\jvm.dll
		sprintf(path, "%s%cbin%c%s%c%s", java_home_path, FILE_SEPARATOR, FILE_SEPARATOR,
			JVM_DLL_CLIENT_PATH, FILE_SEPARATOR, JVM_DLL);
		if (check_file_exist(path))
			return path;

		sprintf(path, "%s%cbin%c%s%c%s", java_home_path, FILE_SEPARATOR, FILE_SEPARATOR,
			JVM_DLL_SERVER_PATH, FILE_SEPARATOR, JVM_DLL);
		if (check_file_exist(path))
			return path;
	}

	//java_home\jre\lib\i386(amd64)\jvm.cfg
	sprintf(jvm_cfg_path, "%s%cjre%clib%c%s%c%s", java_home_path, FILE_SEPARATOR, FILE_SEPARATOR, FILE_SEPARATOR, get_arch(), FILE_SEPARATOR, JVM_CFG);
	if (check_file_exist(jvm_cfg_path))
	{
		//java_home\jre\bin\client(server)\jvm.dll
		sprintf(path, "%s%cjre%cbin%c%s%c%s", java_home_path, FILE_SEPARATOR, FILE_SEPARATOR, FILE_SEPARATOR,
						JVM_DLL_CLIENT_PATH, FILE_SEPARATOR, JVM_DLL);
		if (check_file_exist(path))
			return path;

		sprintf(path, "%s%cjre%cbin%c%s%c%s", java_home_path, FILE_SEPARATOR, FILE_SEPARATOR, FILE_SEPARATOR,
						JVM_DLL_SERVER_PATH, FILE_SEPARATOR, JVM_DLL);
		if (check_file_exist(path))
			return path;
	}

	sprintf(path, "%s%c%s", java_home_path, FILE_SEPARATOR, JVM_DLL);
	if (check_file_exist(path))
		return path;

	free(path);
	return NULL;
}


char* find_public_jre()
{
	HKEY key, subkey;
	char version[50];

	/* Find the current version of the JRE */
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, JRE_KEY, 0, KEY_READ, &key) != 0)
		return NULL;

	if (!get_string_from_registry(key, "CurrentVersion", version, sizeof(version)))
	{
		RegCloseKey(key);
		return NULL;
	}

	/* Find directory where the current version is installed. */
	if (RegOpenKeyEx(key, version, 0, KEY_READ, &subkey) != 0)
	{
		RegCloseKey(key);
		return NULL;
	}

	char* path = malloc(sizeof(char) * MAX_LEN);
	if (!get_string_from_registry(subkey, "JavaHome", path, MAX_LEN))
	{
		RegCloseKey(key);
		RegCloseKey(subkey);
		free(path);
		return NULL;
	}

	RegCloseKey(key);
	RegCloseKey(subkey);
	return path;
}

int get_string_from_registry(HKEY key, const char* name, char* buf, unsigned int buf_size)
{
	DWORD type, size;

	if (RegQueryValueEx(key, name, NULL, &type, NULL, &size) == 0 && type == REG_SZ && (size < (unsigned int)buf_size))
	{
		if (RegQueryValueEx(key, name, NULL, NULL, buf, &size) == 0)
			return JNI_TRUE;
	}
	return JNI_FALSE;
}

enum JvmLoadErrorType load_jvm_dll(const char* jvm_dll_path, const char* class_path, JavaVM** java_vm, JNIEnv** jni_env)
{
	if (!check_file_exist(jvm_dll_path))
		return JVM_DLL_NOT_FOUND;
	HMODULE jvm_dll = LoadLibrary(jvm_dll_path);
	if (jvm_dll == NULL)
	{
		printf("load %s failed.\n", jvm_dll_path);
		return JVM_DLL_LOAD_FAILED;
	}

	JvmCreateFun jvm_create_proc = (JvmCreateFun)GetProcAddress(jvm_dll, JVM_CREATE_FUN);
	if (jvm_create_proc == NULL)
	{
		printf("Can't find the entry of the function \"%s\" in %s\n", JVM_CREATE_FUN, jvm_dll_path);
		return JVM_DLL_ENTRY_NOT_FOUND;
	}

	JavaVMOption vm_options[2];
	JavaVMInitArgs vm_init_args;
	char jvm_class_path[MAX_LEN];
	strcpy(jvm_class_path, "-Djava.class.path=");
	strcat(jvm_class_path, class_path);

	//Init args
	vm_options[0].optionString = jvm_class_path;
	vm_options[1].optionString = "-Dfile.encoding=utf-8";								//set encoding of runtime to UTF-8
	memset(&vm_init_args, 0, sizeof(vm_init_args));
	vm_init_args.version = JNI_VERSION_1_6;
	vm_init_args.nOptions = sizeof(vm_options) / sizeof(JavaVMInitArgs);
	vm_init_args.options = vm_options;

	if (jvm_create_proc(java_vm, (void**)jni_env, (void*)&vm_init_args) != 0)
	{
		fprintf(stderr, "Create JVM failed.\n");
		return JVM_CREATE_FAILED;
	}
	if ((**jni_env)->GetVersion(*jni_env) < JRE_MINIMUM_VERSION_WIN)
	{
		(**java_vm)->DestroyJavaVM(*java_vm);
		return JVM_VERSION_ERROR;
	}
	return JVM_LOAD_SUCCESS;
}
