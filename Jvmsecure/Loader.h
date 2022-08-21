#pragma once
#include <Windows.h>
#include <iostream>
#include <list>
#include "jni.h"
#include <string>
#include "Utils.h"
#include "ClassLoader.h"

class Loader
{
private:
	std::string jarPath;
	std::string mainClassPath;

	HMODULE jvmMod;

	JavaVM* jvm;
	JNIEnv* env;

	std::list<const char*> jvmArgList;
	std::list<const char*> appArgs;

	bool isInit;

public:
	Loader(std::string mainMethod);
	Loader(std::string jarPath, std::string mainMethod);

public:
	void AddToPath(const std::string path);
	void Run();
	void Run(const char* args[], int argsLength);
	void RunFromMemory(const unsigned char data[], int dataSize, bool autoDestroy = true);
	void RunFromMemory(const unsigned char data[], int dataSize, const char* args[], int argsLength, bool autoDestroy = true);
	void DestroyVM();
	void SetJVMArgs(const char* args[], int size);
	void AddJVMArg(const char* arg);
	void AddArg(const char* arg);

private:
	void Init();

};

