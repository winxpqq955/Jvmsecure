#include <iostream>
#include "Utils.h"
#include "Loader.h"
#include "Resource.h"

int main()
{
    std::string dir = GetExeDir();
    std::string gameDir = dir + std::string("\\mc");
    std::string assetsDir = gameDir + std::string("\\assets");
    std::string jarPath = gameDir + std::string("\\resources.jar");
    std::string binariesDir = gameDir + std::string("\\natives");
    std::string libraries = getLibraries(gameDir);

    std::string optClientJar = std::string("-Dminecraft.client.jar=") + jarPath;
    std::string optLibraryPath = std::string("-Djava.library.path=") + binariesDir;
    std::string optClassPath(std::string("-Djava.class.path=") + libraries);

    std::string mainMethod("net/minecraft/client/main/Main");
    std::unique_ptr<Loader> loader(new Loader(jarPath, mainMethod));

    loader->AddJVMArg(optLibraryPath.c_str());
    loader->AddJVMArg(optClientJar.c_str());
    loader->AddJVMArg(optClassPath.c_str());
    loader->AddArg("--username");
    loader->AddArg("anmuxi");
    loader->AddArg("--version");
    loader->AddArg("1.8.8");
    loader->AddArg("--gameDir");
    loader->AddArg(gameDir.c_str());
    loader->AddArg("--assetsDir");
    loader->AddArg(assetsDir.c_str());
    loader->AddArg("--assetIndex");
    loader->AddArg("1.8");
    loader->AddArg("--uuid");
    loader->AddArg("7dc90011f7e74489a10975df0c949993");
    loader->AddArg("--accessToken");
    loader->AddArg("0");

    loader->AddArg("--userProperties");
    loader->AddArg("{}");
    loader->AddArg("--userType");
    loader->AddArg("");
    //loader->Run();
    loader->RunFromMemory(rawData, sizeof(rawData), false);

}