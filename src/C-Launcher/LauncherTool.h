#ifndef LAUNCHER_TOOL_H
#define LAUNCHER_TOOL_H
#ifdef _WIN32

#define FILE_SEPARATOR '\\'
#define PATH_SEPARATOR ';'
#else
#define FILE_SEPARATOR '/'
#define PATH_SEPARATOR ':'
#endif

#ifdef _WIN32
#define JVM_DLL "jvm.dll"
#else
#define JVM_DLL "libjvm.so"
#endif
#define JVM_CFG "jvm.cfg"

#define JAVA_HOME "JAVA_HOME"
#define PATH "PATH"
#define I386 "i386"
#define AMD64 "amd64"
#define BIT32 "32-bit"
#define BIT64 "64-bit"
#define MAX_LEN 768
#define JVM_CREATE_FUN "JNI_CreateJavaVM"
#define JRE_MINIMUM_VERSION_WIN JNI_VERSION_1_6
#define JRE_MINIMUM_VERSION_LINUX 0x00010007						//JRE 1.7
#define TYPING_GAME_JAR_PATH "./Typing-Game.jar"
#include <jni.h>

enum JvmLoadErrorType
{
	JVM_LOAD_SUCCESS,
	JVM_DLL_NOT_FOUND,
	JVM_DLL_LOAD_FAILED,
	JVM_DLL_ENTRY_NOT_FOUND,
	JVM_CREATE_FAILED,
	JVM_VERSION_ERROR
};

int load_jvm(const char* class_path, JavaVM** java_vm, JNIEnv** jni_env);
enum JvmLoadErrorType load_jvm_dll(const char* jvm_dll_path, const char* class_path, JavaVM** java_vm, JNIEnv** jni_env);
int check_file_exist(const char* file_path);
int sub_str(const char* str, int start_pos, int len, char* target_str);
void show_error_dialog(const char* error_msg);
const char* get_arch();
const char* get_bit_version();
const char* get_jre_minimum_version();

#endif
