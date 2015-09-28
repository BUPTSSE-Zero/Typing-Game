#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#include <commctrl.h>
#include <direct.h>
#ifdef _MSC_VER
#pragma comment(lib, "comctl32.lib")
#pragma warning(disable:4996)
#ifndef _DEBUG
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )			//Only show console in debug mode.
#endif //_DEBUG
#endif //_MSC_VER
#else
#define GLOBAL_FONT_SIZE 14
#include <gtk/gtk.h>
#endif //_WIN32

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LauncherTool.h"

#define ERROR_JVM_CREATE_FAILED "Can not create a JVM automatically to run this program.Please check if you have already " \
																"installed %s JRE(%s or later version) and defined the environment variable JAVA_HOME.If you have done them, please check " \
																"if %s exists in PATH or the directory where you installed JRE.\n\n" \
																"Do you want to load %s manually?"

#define ERROR_CLASS_NOT_FOUND "Can not find the class %s in %s."
#define ERROR_METHOD_NOT_FOUND "Can't find the entry of the method \"%s\" in class %s."
#define ERROR_RUNTIME_INIT_FAILED "Load runtime %s failed."

JNIEnv* env;
JavaVM* java_vm;

void manual_load_jvm(const char* jvm_load_error_msg)
{
	char error_msg[MAX_LEN];
	while (1)
	{
		enum JvmLoadErrorType load_result;
		char file_path[MAX_LEN] = {0};
#ifdef _WIN32
		OPENFILENAME ofn = {0};
		char file_filter[50] = {0};
		sprintf(file_filter, "%s%c*.dll", JVM_DLL, '\0');
		if (MessageBox(NULL, jvm_load_error_msg, "Error", MB_YESNO | MB_ICONERROR) != IDYES)
			exit(EXIT_FAILURE);
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = file_filter;
		ofn.lpstrFile = file_path;
		ofn.nMaxFile = sizeof(file_path);
		ofn.nFilterIndex = 0;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR;
		if (GetOpenFileName(&ofn) == FALSE)
			continue;
#else
		GtkDialog* dialog = GTK_DIALOG(gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_YES_NO, jvm_load_error_msg, NULL));
    gtk_window_set_title(GTK_WINDOW(dialog), "Error");
    if(gtk_dialog_run(dialog) != GTK_RESPONSE_YES)
      exit(EXIT_FAILURE);
    gtk_widget_destroy(GTK_WIDGET(dialog));
    GtkFileChooserDialog* file_chooser_dialog = GTK_FILE_CHOOSER_DIALOG(gtk_file_chooser_dialog_new("Browse", NULL, GTK_FILE_CHOOSER_ACTION_OPEN,
                                                                           GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                                                           GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL));
    GtkFileFilter* file_filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(file_filter, "*.so");
    gtk_file_filter_set_name(file_filter, JVM_DLL);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_chooser_dialog), file_filter);
    int ret = gtk_dialog_run(GTK_DIALOG(file_chooser_dialog));
    gtk_widget_hide(GTK_WIDGET(file_chooser_dialog));
    if(ret != GTK_RESPONSE_ACCEPT)
    {
      gtk_widget_destroy(GTK_WIDGET(file_chooser_dialog));
      continue;
    }
    strcpy(file_path, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser_dialog)));
    gtk_widget_destroy(GTK_WIDGET(file_chooser_dialog));
#endif
		load_result = load_jvm_dll(file_path, TYPING_GAME_JAR_PATH, &java_vm, &env);
		if (load_result == JVM_LOAD_SUCCESS)
			return;
		switch (load_result)
		{
			case JVM_DLL_NOT_FOUND:
				sprintf(error_msg, "The %s doesn't exist.\n", JVM_DLL);
				break;
			case JVM_DLL_LOAD_FAILED:
				sprintf(error_msg, "Load %s failed.Please check if it's a valid %s %s.\n", JVM_DLL, get_bit_version(), JVM_DLL);
				break;
			case JVM_DLL_ENTRY_NOT_FOUND:
				sprintf(error_msg, "The entry of the function \"%s\" can't be found in %s.\n", JVM_CREATE_FUN, JVM_DLL);
				break;
			case JVM_CREATE_FAILED:
				sprintf(error_msg, "Create JVM failed.Try again.\n");
				break;
			case JVM_VERSION_ERROR:
				sprintf(error_msg, "JRE version error.The minimum version of JRE required is %s.\n", get_jre_minimum_version());
				break;
			default:
				sprintf(error_msg, "Unknown error.\n");
				break;
		}
		strcat(error_msg, JVM_DLL);
		strcat(error_msg, " path:");
		strcat(error_msg, file_path);
		show_error_dialog(error_msg);
	}
}

int main()
{
#ifdef _WIN32
	InitCommonControls();													//init win32 widgets in system theme.
	char exec_path[MAX_LEN];
	GetModuleFileName(NULL, exec_path, sizeof(exec_path));
	*(strrchr(exec_path, FILE_SEPARATOR)) = '\0';
	chdir(exec_path);
#else
  gtk_init(NULL, NULL);                         //init gtk library.
#endif // _WIN32

	char error_msg[MAX_LEN];
	if (!load_jvm(TYPING_GAME_JAR_PATH, &java_vm, &env))
	{
		sprintf(error_msg, ERROR_JVM_CREATE_FAILED, get_bit_version(), get_jre_minimum_version(), JVM_DLL, JVM_DLL);
		manual_load_jvm(error_msg);
	}

	jclass main_class = (*env)->FindClass(env, CLASS_MAIN_INTERFACE);
	if (main_class == NULL)
	{
    sprintf(error_msg, ERROR_CLASS_NOT_FOUND, CLASS_MAIN_INTERFACE, TYPING_GAME_JAR_PATH);
		show_error_dialog(error_msg);
		return -1;
	}

	jmethodID main_method_id = (*env)->GetStaticMethodID(env, main_class, METHOD_SHOW, "()V");
	if (main_method_id == NULL)
	{
    sprintf(error_msg, ERROR_METHOD_NOT_FOUND, METHOD_SHOW, CLASS_MAIN_INTERFACE);
		show_error_dialog(error_msg);
		return -1;
	}

  const char* default_font = NULL;
  jclass settings_class = (*env)->FindClass(env, CLASS_GLOBAL_SETTINGS);
	if (settings_class != NULL)
	{
    //init runtime
    jmethodID init_runtime_method_id = (*env)->GetStaticMethodID(env, settings_class, METHOD_INIT_RUNTIME, "(Ljava/lang/String;)Z");
    if(init_runtime_method_id == NULL)
    {
      sprintf(error_msg, ERROR_METHOD_NOT_FOUND, METHOD_INIT_RUNTIME, CLASS_GLOBAL_SETTINGS);
      show_error_dialog(error_msg);
      return -1;
    }

    jstring runtime_path = (*env)->NewStringUTF(env, TYPING_GAME_RUNTIME);
    jboolean result = (*env)->CallStaticBooleanMethod(env, settings_class, init_runtime_method_id, runtime_path);
    (*env)->DeleteGlobalRef(env, runtime_path);
    if(!result)
    {
      sprintf(error_msg, ERROR_RUNTIME_INIT_FAILED, TYPING_GAME_RUNTIME);
      show_error_dialog(error_msg);
      return 1;
    }

		jmethodID set_font_method_id = (*env)->GetStaticMethodID(env, settings_class, METHOD_SET_UI_FONT, "(Ljava/lang/String;)V");
		if (set_font_method_id != NULL)
		{
#ifdef _WIN32
			LOGFONT system_font;
			SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(system_font), &system_font, 0);
			default_font = system_font.lfFaceName;
#else
      GtkStyle* default_style = gtk_style_new();
      default_font = pango_font_description_get_family(default_style->font_desc);
      char gtk_font[MAX_LEN];
      sprintf(gtk_font, "%s %d", default_font, GLOBAL_FONT_SIZE);
      GtkSettings* gtk_settings = gtk_settings_get_default();
      gtk_settings_set_string_property(gtk_settings, "gtk-font-name", gtk_font, "San 12");
#endif // _WIN32
      if(default_font)
      {
        jstring font_family = (*env)->NewStringUTF(env, default_font);
        (*env)->CallStaticVoidMethod(env, settings_class, set_font_method_id, font_family);
        (*env)->DeleteGlobalRef(env, font_family);
      }
		}
	}else{
    sprintf(error_msg, ERROR_CLASS_NOT_FOUND, CLASS_GLOBAL_SETTINGS, TYPING_GAME_JAR_PATH);
    show_error_dialog(error_msg);
    return -1;
	}

	(*env)->CallStaticVoidMethod(env, main_class, main_method_id);

	(*java_vm)->DestroyJavaVM(java_vm);
	return 0;
}
