#ifdef _WIN32
#include <windows.h>
#define PATH_SEPARATOR ';'
#ifdef _MSC_VER
#pragma comment(lib, "jvm.lib")
#endif
#else
#define PATH_SEPARATOR ':'
#endif
#include <jni.h>
#include <stdio.h>

int main()
{
	JavaVMOption vm_options[2];
	JNIEnv* env;
	JavaVM* java_vm;
	JavaVMInitArgs vm_init_args;

	//Init args
	vm_options[0].optionString = "-Djava.class.path=./Typing-Game.jar";
	vm_options[1].optionString = "-Dfile.encoding=utf-8";								//set encoding of runtime to UTF-8
	memset(&vm_init_args, 0, sizeof(vm_init_args));
	vm_init_args.version = JNI_VERSION_1_6;
	vm_init_args.nOptions = 2;
	vm_init_args.options = vm_options;
	
	if (JNI_CreateJavaVM(&java_vm, (void**)&env, (void*)&vm_init_args) != 0)
	{
		printf("Create JVM failed!\n");
		return -1;
	}

	jclass main_class = (*env)->FindClass(env, "buptsse/zero/MainInterface");
	if (main_class == NULL)
	{
		printf("Can't find the main class buptsse.zero.MainInterface.");
		return -1;
	}

	//"([Ljava/lang/String;)V" represents the argument type is Sting[], and returning value type is void.
	jmethodID main_method_id = (*env)->GetStaticMethodID(env, main_class, "main", "([Ljava/lang/String;)V");
	if (main_method_id == NULL)
	{
		printf("Can't find the main method in class buptsse.zero.MainInterface");
		return -1;
	}
	(*env)->CallStaticVoidMethod(env, main_class, main_method_id);
	
	(*java_vm)->DestroyJavaVM(java_vm);
	return 0;
}
