#pragma once
#include <windows.h>
#include <string>
#include <sstream>
#include <vector>
#include "jni.h"


std::string jstring2string(JNIEnv* env, jstring jStr);

std::string GetExeDir();

std::string GetCurrentExeName();

jobjectArray CharArrayToJavaStringArray(JNIEnv* env, const char* data[], int size);

jbyteArray CharArrayToJavaByteArray(JNIEnv* env, const unsigned char data[], int len);

std::string getLibraries(std::string gameDir);