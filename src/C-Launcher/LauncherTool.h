#ifndef LAUNCHER_TOOL_H
#define LAUNCHER_TOOL_H
#ifdef _WIN32
#define FILE_SEPARATOR '\\'
#define PATH_SEPARATOR ';'
#else
#define FILE_SEPARATOR '/'
#define PATH_SEPARATOR ':'
#endif

#define JAVA_HOME "JAVA_HOME"

#include <jni.h>

int load_jvm(const char* class_path, JavaVM** java_vm, JNIEnv** jni_env);

#endif
