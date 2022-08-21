#pragma once
#include <jni.h>
#include <string>

class ClassLoader
{
private:
	JNIEnv* env;
	jobject classes;
	jobject classLoader;
	jobject stream;

public:
	ClassLoader(JNIEnv* env, jobject stream);
	void load();

private:
	jbyteArray readClass(jobject stream);
	jobject loadClass(jstring name);
};