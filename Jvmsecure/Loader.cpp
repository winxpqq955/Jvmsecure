#include "Loader.h"

Loader::Loader(const std::string mainClass) {
	this->Loader::Loader(GetCurrentExeName(), mainClass);
}

Loader::Loader(const std::string jarPath, const std::string mainClass) {
	this->jarPath = jarPath;
	this->mainClassPath = mainClass;
	this->isInit = false;
	this->jvm = nullptr;
}

void Loader::Init() {
	if (isInit) return;

	jvmMod = LoadLibraryA((GetExeDir() + std::string("\\runtime\\bin\\server\\jvm.dll")).c_str());

	if (jvmMod == nullptr) {
		std::cout << "Error: " << "jvm.dll could not be loaded" << std::endl;
		exit(EXIT_FAILURE);
	}

	typedef jint(JNICALL* PtrCreateJavaVM)(JavaVM**, void**, void*);
	PtrCreateJavaVM ptrCreateJavaVM = (PtrCreateJavaVM)GetProcAddress(jvmMod, "JNI_CreateJavaVM");

	JavaVMInitArgs vmArgs;
	JavaVMOption* vmOptions = new JavaVMOption[32];
	std::string optClassPath = std::string("-Djava.class.path=") + this->jarPath;

	int argLength = 0;
	vmOptions[argLength].optionString = (char*)optClassPath.c_str();

	for (const char* arg : jvmArgList) {
		argLength++;
		vmOptions[argLength].optionString = (char*)arg;
	}

	vmArgs.version = JNI_VERSION_1_8;
	vmArgs.nOptions = argLength + 1;
	vmArgs.options = vmOptions;
	vmArgs.ignoreUnrecognized = JNI_FALSE;

	jint vm = ptrCreateJavaVM(&jvm, (void**)&env, &vmArgs);

	if (vm != JNI_OK) {
		std::cout << "Error: " << "Can't create java vm" << std::endl;
		exit(EXIT_FAILURE);
	}
	AddToPath(this->jarPath);
	isInit = true;
}

void Loader::Run() {
	Run(NULL, 0);
}

void Loader::AddToPath(const std::string path) {
	const std::string urlPath = "file://" + path;
	jclass class_loader = env->FindClass("java/lang/ClassLoader");
	jmethodID system_class_loader = env->GetStaticMethodID(class_loader, "getSystemClassLoader", "()Ljava/lang/ClassLoader;");
	jobject class_loader_instance = env->CallStaticObjectMethod(class_loader, system_class_loader);
	jclass url_class_loader = env->FindClass("java/net/URLClassLoader");
	jmethodID add_url = env->GetMethodID(url_class_loader, "addURL", "(Ljava/net/URL;)V");
	jclass url_class = env->FindClass("java/net/URL");
	jmethodID url_constructor = env->GetMethodID(url_class, "<init>", "(Ljava/lang/String;)V");
	jobject url_instance = env->NewObject(url_class, url_constructor, env->NewStringUTF(urlPath.c_str()));
	env->CallVoidMethod(class_loader_instance, add_url, url_instance);
}

void Loader::Run(const char* args[], int argsLength) {
	Init();

	jclass mainClass = env->FindClass(mainClassPath.c_str());

	if (mainClass == nullptr) {
		std::cout << "Error: " << "Main class not found" << std::endl;
		exit(EXIT_FAILURE);
	}

	jmethodID mainMethod = env->GetStaticMethodID(mainClass, "main", "([Ljava/lang/String;)V");

	if (mainMethod == nullptr) {
		std::cout << "Error: " << "Main method not found" << std::endl;
		exit(EXIT_FAILURE);
	}

	const char* newArgs[1024];
	int newLength = 0;

	for (int i = 0; i < argsLength; i++) {
		newArgs[i] = args[i];
		newLength++;
	}

	for (const char* arg : appArgs) {
		newArgs[newLength] = arg;
		newLength++;
	}

	for (int i = 0; i < newLength; i++) {
		std::cout << "Arg: " << newArgs[i] << std::endl;
	}

	env->CallStaticVoidMethod(mainClass, mainMethod, CharArrayToJavaStringArray(env, newArgs, newLength));
}

void Loader::RunFromMemory(const unsigned char data[], int dataSize, bool autoDestroy) {
	RunFromMemory(data, dataSize, NULL, 0, autoDestroy);
}

void Loader::RunFromMemory(const unsigned char data[], int dataSize, const char* args[], int argsLength, bool autoDestroy) {
	Init();

	jclass byteArrayinputStream = env->FindClass("java/io/ByteArrayInputStream");
	jmethodID byteArrayinputStreamConstructor = env->GetMethodID(byteArrayinputStream, "<init>", "([B)V");
	jobject stream = env->NewObject(byteArrayinputStream, byteArrayinputStreamConstructor, CharArrayToJavaByteArray(env, data, dataSize));

	std::unique_ptr<ClassLoader> classLoader(new ClassLoader(env, stream));
	classLoader->load();

	jclass mainClass = env->FindClass(mainClassPath.c_str());
	jmethodID mainMethod = env->GetStaticMethodID(mainClass, "main", "([Ljava/lang/String;)V");

	const char* newArgs[1024];
	int newLength = 0;

	for (int i = 0; i < argsLength; i++) {
		newArgs[i] = args[i];
		newLength++;
	}

	for (const char* arg : appArgs) {
		newArgs[newLength] = arg;
		newLength++;
	}

	jobjectArray argss = CharArrayToJavaStringArray(env, newArgs, newLength);

	env->CallStaticVoidMethod(mainClass, mainMethod, argss);
	if (autoDestroy) DestroyVM();
}

void Loader::DestroyVM() {
	jvm->DestroyJavaVM();
}

void Loader::SetJVMArgs(const char* args[], int size) {
	jvmArgList.clear();
	for (int i = 0; i < size; i++) {
		jvmArgList.push_back(args[i]);
	}
}

void Loader::AddJVMArg(const char* arg) {
	jvmArgList.push_back(arg);
}

void Loader::AddArg(const char* arg) {
	appArgs.push_back(arg);
}