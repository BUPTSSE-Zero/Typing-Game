#include "JNI_MainInterface.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <gtk/gtk.h>
#endif

#define MAX_LEN 512

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

JNIEXPORT jstring JNICALL Java_buptsse_zero_MainInterface_openFile(JNIEnv* env, jobject obj, jstring file_suffix)
{
	char file_filter[MAX_LEN] = {0};
	char file_path[MAX_LEN] = {0};
	int ret = JNI_FALSE;
	const char* file_suffix_utf8 = (*env)->GetStringUTFChars(env, file_suffix, JNI_FALSE);
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
#endif
	if (ret)
		return (*env)->NewStringUTF(env, file_path);
	return (*env)->NewStringUTF(env, NULL);
}
