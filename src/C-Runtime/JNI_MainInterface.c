#include "JNI_MainInterface.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <gtk/gtk.h>
#endif

#define MAX_LEN 512

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

char file_filter[MAX_LEN] = {0};
char file_path[MAX_LEN] = {0};
int ret = JNI_FALSE;

#ifndef _WIN32
#define FILE_DIALOG_FONT_SIZE 12
GMutex GUIMutex;
GCond GUICond;
gboolean exec_show_file_browser(gpointer file_suffix_utf8)
{
  ret = JNI_FALSE;
  GtkDialog* file_chooser_dialog = GTK_DIALOG(gtk_file_chooser_dialog_new("Browse", NULL, GTK_FILE_CHOOSER_ACTION_OPEN,
                                                              GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                                              GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL));
  GtkFileFilter* gtk_file_filter = gtk_file_filter_new();
  sprintf(file_filter, "*.%s", (char*)file_suffix_utf8);
  gtk_file_filter_add_pattern(gtk_file_filter, file_filter);
  gtk_file_filter_set_name(gtk_file_filter, file_filter);
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_chooser_dialog), gtk_file_filter);
  int response = gtk_dialog_run(file_chooser_dialog);
  gdk_window_hide(gtk_widget_get_window(GTK_WIDGET(file_chooser_dialog)));
  if(response != GTK_RESPONSE_ACCEPT)
    ret = JNI_FALSE;
  else
  {
    ret = JNI_TRUE;
    strcpy(file_path, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser_dialog)));
  }
  gdk_window_destroy(gtk_widget_get_window(GTK_WIDGET(file_chooser_dialog)));
  g_cond_signal(&GUICond);
  return FALSE;
}
#endif // _WIN32

JNIEXPORT jstring JNICALL Java_buptsse_zero_MainInterface_openFile(JNIEnv* env, jclass obj, jstring file_suffix)
{
	ret = JNI_FALSE;
	const char* file_suffix_utf8 = (*env)->GetStringUTFChars(env, file_suffix, JNI_FALSE);
	memset(file_filter, 0, sizeof(file_filter));
	memset(file_path, 0, sizeof(file_path));
#ifdef _WIN32
	sprintf(file_filter, "*.%s%c*.%s", file_suffix_utf8, '\0', file_suffix_utf8);
	OPENFILENAME ofn = {0};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = file_filter;
	ofn.lpstrFile = file_path;
	ofn.nMaxFile = sizeof(file_path);
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR;
	ret = GetOpenFileName(&ofn);
#else
  g_mutex_init(&GUIMutex);
  g_cond_init(&GUICond);
  gdk_threads_add_idle((GSourceFunc)exec_show_file_browser, file_suffix_utf8);
  g_cond_wait(&GUICond, &GUIMutex);
#endif
	if (ret)
		return (*env)->NewStringUTF(env, file_path);
	return (*env)->NewStringUTF(env, NULL);
}
