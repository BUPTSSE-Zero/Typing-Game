#include "LauncherTool.h"
#include <dirent.h>
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
