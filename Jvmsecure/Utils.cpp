#include "Utils.h"


std::string jstring2string(JNIEnv* env, jstring jStr) {
	if (!jStr)
		return "";

	const jclass stringClass = env->GetObjectClass(jStr);
	const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
	const jbyteArray stringJbytes = (jbyteArray)env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));

	size_t length = (size_t)env->GetArrayLength(stringJbytes);
	jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

	std::string ret = std::string((char*)pBytes, length);
	env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

	env->DeleteLocalRef(stringJbytes);
	env->DeleteLocalRef(stringClass);
	return ret;
}

std::string GetExeDir() {
    char result[MAX_PATH];
	std::string dir = std::string(result, GetModuleFileNameA(NULL, result, MAX_PATH));
    size_t last = dir.find_last_of("\\");

    return dir.substr(0, last);
}

std::string GetCurrentExeName() {
    char name[MAX_PATH];
    GetModuleFileNameA(nullptr, name, MAX_PATH);
    return name;
}

jobjectArray CharArrayToJavaStringArray(JNIEnv* env, const char* data[], int size) {
    jobjectArray array = env->NewObjectArray(size, env->FindClass("java/lang/String"), env->NewStringUTF(""));

    for (int i = 0; i < size; i++) env->SetObjectArrayElement(array, i, env->NewStringUTF(data[i]));

    return array;
}

jbyteArray CharArrayToJavaByteArray(JNIEnv* env, const unsigned char data[], int len) {
    jbyteArray array = env->NewByteArray(len);
    jbyte* bytes = new jbyte[len];

    for (int i = 0; i < len; i++) bytes[i] = (jbyte)((unsigned char)data[i]);

    env->SetByteArrayRegion(array, 0, len, bytes);

    return array;
}

std::string getLibraries(std::string gameDir) {
	std::string libDir = gameDir + std::string("\\libraries\\");
	std::vector<std::string> libraries;

	libraries.push_back(libDir + (char*)"com\\mojang\\netty\\1.8.8\\netty-1.8.8.jar");
	libraries.push_back(libDir + (char*)"oshi-project\\oshi-core\\1.1\\oshi-core-1.1.jar");
	libraries.push_back(libDir + (char*)"net\\java\\dev\\jna\\jna\\3.4.0\\jna-3.4.0.jar");
	libraries.push_back(libDir + (char*)"net\\java\\dev\\jna\\platform\\3.4.0\\platform-3.4.0.jar");
	libraries.push_back(libDir + (char*)"com\\ibm\\icu\\icu4j-core-mojang\\51.2\\icu4j-core-mojang-51.2.jar");
	libraries.push_back(libDir + (char*)"net\\sf\\jopt-simple\\jopt-simple\\4.6\\jopt-simple-4.6.jar");
	libraries.push_back(libDir + (char*)"com\\paulscode\\codecjorbis\\20101023\\codecjorbis-20101023.jar");
	libraries.push_back(libDir + (char*)"com\\paulscode\\codecwav\\20101023\\codecwav-20101023.jar");
	libraries.push_back(libDir + (char*)"com\\paulscode\\libraryjavasound\\20101123\\libraryjavasound-20101123.jar");
	libraries.push_back(libDir + (char*)"com\\paulscode\\librarylwjglopenal\\20100824\\librarylwjglopenal-20100824.jar");
	libraries.push_back(libDir + (char*)"com\\paulscode\\soundsystem\\20120107\\soundsystem-20120107.jar");
	libraries.push_back(libDir + (char*)"io\\netty\\netty-all\\4.0.23.Final\\netty-all-4.0.23.Final.jar");
	libraries.push_back(libDir + (char*)"com\\google\\guava\\guava\\17.0\\guava-17.0.jar");
	libraries.push_back(libDir + (char*)"org\\apache\\commons\\commons-lang3\\3.3.2\\commons-lang3-3.3.2.jar");
	libraries.push_back(libDir + (char*)"commons-io\\commons-io\\2.4\\commons-io-2.4.jar");
	libraries.push_back(libDir + (char*)"commons-codec\\commons-codec\\1.9\\commons-codec-1.9.jar");
	libraries.push_back(libDir + (char*)"net\\java\\jinput\\jinput\\2.0.5\\jinput-2.0.5.jar");
	libraries.push_back(libDir + (char*)"net\\java\\jutils\\jutils\\1.0.0\\jutils-1.0.0.jar");
	libraries.push_back(libDir + (char*)"com\\google\\code\\gson\\gson\\2.2.4\\gson-2.2.4.jar");
	libraries.push_back(libDir + (char*)"com\\mojang\\authlib\\1.5.21\\authlib-1.5.21.jar");
	libraries.push_back(libDir + (char*)"com\\mojang\\realms\\1.7.39\\realms-1.7.39.jar");
	libraries.push_back(libDir + (char*)"org\\apache\\commons\\commons-compress\\1.8.1\\commons-compress-1.8.1.jar");
	libraries.push_back(libDir + (char*)"org\\apache\\httpcomponents\\httpclient\\4.3.3\\httpclient-4.3.3.jar");
	libraries.push_back(libDir + (char*)"commons-logging\\commons-logging\\1.1.3\\commons-logging-1.1.3.jar");
	libraries.push_back(libDir + (char*)"org\\apache\\httpcomponents\\httpcore\\4.3.2\\httpcore-4.3.2.jar");
	libraries.push_back(libDir + (char*)"org\\apache\\logging\\log4j\\log4j-api\\2.0-beta9\\log4j-api-2.0-beta9.jar");
	libraries.push_back(libDir + (char*)"org\\apache\\logging\\log4j\\log4j-core\\2.0-beta9\\log4j-core-2.0-beta9.jar");
	libraries.push_back(libDir + (char*)"org\\lwjgl\\lwjgl\\lwjgl\\2.9.4-nightly-20150209\\lwjgl-2.9.4-nightly-20150209.jar");
	libraries.push_back(libDir + (char*)"org\\lwjgl\\lwjgl\\lwjgl_util\\2.9.4-nightly-20150209\\lwjgl_util-2.9.4-nightly-20150209.jar");
	libraries.push_back(libDir + (char*)"tv\\twitch\\twitch\\6.5\\twitch-6.5.jar");

	std::stringstream out;

	for (auto& str : libraries) {
		out << ";" << str;
	}

	return out.str().substr(1, out.str().size());
}