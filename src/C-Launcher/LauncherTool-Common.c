#ifdef _WIN32
#include <windows.h>
#else
#include <gtk/gtk.h>
#endif //_WIN32

#include "LauncherTool.h"
#include <limits.h>
#include <sys/stat.h>

int check_file_exist(const char* file_path)
{
	struct stat file_stat;
  if (stat(file_path, &file_stat) != 0)
    return JNI_FALSE;
  if (file_stat.st_mode & S_IFDIR)
    return JNI_FALSE;
  return JNI_TRUE;
}

int sub_str(const char* str, int start_pos, int len, char* target_str)
{
	if (len <= 0)
		return JNI_FALSE;
	int c = 0, i;
	for (i = start_pos; i < start_pos + len; i++)
		target_str[c++] = str[i];
	target_str[c] = '\0';
	return JNI_TRUE;
}

const char* get_arch()
{
	int bits = CHAR_BIT * sizeof(void*);
	if (bits == 32)
		return I386;
	else if (bits == 64)
		return AMD64;
	return I386;
}

void show_error_dialog(const char* error_msg)
{
#ifdef _WIN32
	MessageBox(NULL, error_msg, "Error", MB_ICONERROR | MB_OK);
#else
  GtkDialog* dialog = GTK_DIALOG(gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, error_msg));
  gtk_window_set_title(GTK_WINDOW(dialog), "Error");
  gtk_dialog_run(dialog);
  gtk_widget_destroy(GTK_WIDGET(dialog));
#endif // _WIN32
}
