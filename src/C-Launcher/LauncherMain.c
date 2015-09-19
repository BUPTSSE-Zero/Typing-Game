#ifdef _WIN32
#include <windows.h>
#include <commctrl.h>
#ifdef _MSC_VER
#pragma comment(lib, "comctl32.lib")
#ifndef _DEBUG
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )			//Only show console in debug mode.
#endif
#endif
#endif
#include <jni.h>
#include <stdio.h>
#include <string.h>
#include "LauncherTool.h"

#define ERROR_JVM_CREATE_FAILED "Can not create a JVM to run this program.Please check if you have \
																installed JDK or JRE.If you have installed either of them, please check \
																if jvm.dll(Windows) or jvm.so(Linux) exists in PATH or the directory where you installed JDK(JRE)."

#define ERROR_MAIN_CLASS_NOT_FOUND "Can not find the class buptsse.zero.MainInterface in Typing-Game.jar"
#define ERROR_MAIN_METHOD_NOT_FOUND "Can't find the entry method \"show()\" in class buptsse.zero.MainInterface"

void show_error_dialog(const char* error_msg)
{
#ifdef _WIN32
	MessageBox(NULL, error_msg, "Error", MB_ICONERROR | MB_OK);
#endif
}

int main()
{
#ifdef _WIN32																		//init win32 widgets in system theme.
	InitCommonControls();
#endif

	JNIEnv* env;
	JavaVM* java_vm;

	if (!load_jvm("./Typing-Game.jar", &java_vm, &env))
	{
		show_error_dialog(ERROR_JVM_CREATE_FAILED);
		return -1;
	}

	jclass main_class = (*env)->FindClass(env, "buptsse/zero/MainInterface");
	if (main_class == NULL)
	{
		show_error_dialog(ERROR_MAIN_CLASS_NOT_FOUND);
		return -1;
	}

	jmethodID main_method_id = (*env)->GetStaticMethodID(env, main_class, "show", "()V");
	if (main_method_id == NULL)
	{
		show_error_dialog(ERROR_MAIN_METHOD_NOT_FOUND);
		return -1;
	}

#ifdef _WIN32
	jclass settings_class = (*env)->FindClass(env, "buptsse/zero/GlobalSettings");
	if (settings_class != NULL)
	{
		jmethodID set_font_method_id = (*env)->GetStaticMethodID(env, settings_class, "setUIFont", "(Ljava/lang/String;)V");
		if (set_font_method_id != NULL)
		{
			LOGFONT system_font;
			SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(system_font), &system_font, 0);
			jstring font_family = (*env)->NewStringUTF(env, system_font.lfFaceName);
			(*env)->CallStaticVoidMethod(env, settings_class, set_font_method_id, font_family);
			(*env)->DeleteGlobalRef(env, font_family);
		}
	}
#endif

	(*env)->CallStaticVoidMethod(env, main_class, main_method_id);

	(*java_vm)->DestroyJavaVM(java_vm);
	return 0;
}
