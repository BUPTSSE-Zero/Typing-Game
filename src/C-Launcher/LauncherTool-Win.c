#include "LauncherTool.h"
#include <windows.h>
#include <winreg.h>
#include <string.h>
#include <stdio.h>
#define MAX_LEN 512
#define JVM_DLL_CLIENT_PATH "\\bin\\client\\jvm.dll"
#define JVM_DLL_SERVER_PATH "\\bin\\server\\jvm.dll"
#define JRE_KEY "Software\\JavaSoft\\Java Runtime Environment"

char* find_jvm_dll(const char* java_home_path);
char* find_public_jre();
int get_string_from_registry(HKEY key, const char* name, char* buf, unsigned int buf_size);

int load_jvm(const char* class_path, JavaVM** java_vm, JNIEnv** jni_env)
{
	char java_home[MAX_LEN];
	char java_home_path[MAX_LEN];
	char* jvm_dll_path;

	int find_flag = JNI_FALSE;
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
						find_flag = JNI_TRUE;
						break;
					}
				}
			}
		}
	}

	if (!find_flag && GetEnvironmentVariable(PATH, java_home, sizeof(java_home)) > 0)
	{
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
						find_flag = JNI_TRUE;
						break;
					}
				}
			}
		}
	}

	if (!find_flag)
	{
		char* jre_path = find_public_jre();
		if (jre_path == NULL)
			return JNI_FALSE;
		jvm_dll_path = find_jvm_dll(jre_path);
		if (jvm_dll_path != NULL)
		{
			free(jre_path);
			find_flag = JNI_TRUE;
		}
	}

	if (!find_flag)
		return JNI_FALSE;

	HMODULE jvm_dll = LoadLibrary(jvm_dll_path);
	if (jvm_dll == NULL)
		return JNI_FALSE;
	printf("jvm.dll found:%s\n", jvm_dll_path);
	free(jvm_dll_path);

	jint(JNICALL *jvm_create_proc)(JavaVM**, void**, void*);
	jvm_create_proc = GetProcAddress(jvm_dll, "JNI_CreateJavaVM");
	if (jvm_create_proc == NULL)
		return JNI_FALSE;

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
	vm_init_args.nOptions = 2;
	vm_init_args.options = vm_options;

	if (jvm_create_proc(java_vm, (void**)jni_env, (void*)&vm_init_args) != 0)
  {
    fprintf(stderr, "Create JVM failed.\n");
		return JNI_FALSE;
  }
	return JNI_TRUE;
}

char* find_jvm_dll(const char* java_home_path)
{
	char* path = malloc(sizeof(char) * MAX_LEN);
	strcpy(path, java_home_path);
	strcat(path, "\\jvm.dll");
	if (check_file_exist(path))
		return path;

	strcpy(path, java_home_path);
	strcat(path, JVM_DLL_CLIENT_PATH);
	if (check_file_exist(path))
		return path;

	strcpy(path, java_home_path);
	strcat(path, JVM_DLL_SERVER_PATH);
	if (check_file_exist(path))
		return path;

	strcpy(path, java_home_path);
	strcat(path, "\\jre");
	strcat(path, JVM_DLL_CLIENT_PATH);
	if (check_file_exist(path))
		return path;

	strcpy(path, java_home_path);
	strcat(path, "\\jre");
	strcat(path, JVM_DLL_SERVER_PATH);
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
