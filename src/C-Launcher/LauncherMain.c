#ifdef _WIN32
#include <windows.h>
#include <commctrl.h>
#ifdef _MSC_VER
#pragma comment(lib, "comctl32.lib")
#ifndef _DEBUG
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )			//Only show console in debug mode.
#endif //_DEBUG
#endif //_MSC_VER
#else
#include <gtk/gtk.h>
#endif //_WIN32

#include <jni.h>
#include <stdio.h>
#include <string.h>
#include "LauncherTool.h"

#define ERROR_JVM_CREATE_FAILED "Can not create a JVM to run this program.Please check if you have already " \
																"installed JDK or JRE and configured the environment variable JAVA_HOME.If you have done them, please check " \
																"if jvm.dll(Windows) or libjvm.so(Linux) exists in PATH or the directory where you installed JDK(JRE)."

#define ERROR_MAIN_CLASS_NOT_FOUND "Can not find the class buptsse.zero.MainInterface in Typing-Game.jar"
#define ERROR_MAIN_METHOD_NOT_FOUND "Can't find the entry method \"show()\" in class buptsse.zero.MainInterface"

int main()
{
#ifdef _WIN32																		//init win32 widgets in system theme.
	InitCommonControls();
#else
  gtk_init(NULL, NULL);                         //init gtk library.
#endif // _WIN32

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

  const char* default_font = NULL;
  jclass settings_class = (*env)->FindClass(env, "buptsse/zero/GlobalSettings");
	if (settings_class != NULL)
	{
		jmethodID set_font_method_id = (*env)->GetStaticMethodID(env, settings_class, "setUIFont", "(Ljava/lang/String;)V");
		if (set_font_method_id != NULL)
		{
#ifdef _WIN32
			LOGFONT system_font;
			SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(system_font), &system_font, 0);
			default_font = system_font.lfFaceName;
#else
      GtkStyle* default_style = gtk_style_new();
      default_font = pango_font_description_get_family(default_style->font_desc);
#endif // _WIN32
      if(default_font)
      {
        printf("Default font:%s\n", default_font);
        jstring font_family = (*env)->NewStringUTF(env, default_font);
        (*env)->CallStaticVoidMethod(env, settings_class, set_font_method_id, font_family);
        (*env)->DeleteGlobalRef(env, font_family);
      }
		}
	}

	(*env)->CallStaticVoidMethod(env, main_class, main_method_id);

	(*java_vm)->DestroyJavaVM(java_vm);
	return 0;
}
