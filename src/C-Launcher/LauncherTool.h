﻿#ifndef LAUNCHER_TOOL_H
#define LAUNCHER_TOOL_H
#ifdef _WIN32

#define FILE_SEPARATOR '\\'
#define PATH_SEPARATOR ';'
#else
#define FILE_SEPARATOR '/'
#define PATH_SEPARATOR ':'
#endif

#define JAVA_HOME "JAVA_HOME"
#define PATH "PATH"
#define I386 "i386"
#define AMD64 "amd64"

#include <jni.h>

int load_jvm(const char* class_path, JavaVM** java_vm, JNIEnv** jni_env);
int check_file_exist(const char* file_path);
int sub_str(const char* str, int start_pos, int len, char* target_str);
void show_error_dialog(const char* error_msg);
const char* get_arch();

#endif
