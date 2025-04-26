#include <Engine/ResourceTypes/ResourceManager.h>

#include <Engine/Application.h>
#include <Engine/Diagnostics/Log.h>
#include <Engine/Filesystem/File.h>
#include <Engine/Filesystem/VFS/VirtualFileSystem.h>

#define RESOURCES_VFS_NAME "main"

#define RESOURCES_DIR_PATH "Resources"

bool ResourceManager::UsingDataFolder = true;
bool ResourceManager::UsingModPack = false;

std::vector<ModInfo> ResourceManager::Mods;
INI*                 ResourceManager::ModConfig = NULL;

VirtualFileSystem* vfs = nullptr;
VFSProvider* mainResource = nullptr;

const char* data_files[] = {"Data.hatch", "Game.hatch", TARGET_NAME ".hatch"};

const char* FindDataFile() {
	for (size_t i = 0; i < sizeof(data_files) / sizeof(data_files[0]); i++) {
		const char* filename = data_files[i];

		if (File::Exists(filename)) {
			return filename;
		}
	}

	return nullptr;
}

bool ResourceManager::Init(const char* filename) {
	vfs = new VirtualFileSystem();

	if (filename != NULL && File::Exists(filename)) {
		Log::Print(Log::LOG_IMPORTANT, "Loading \"%s\"...", filename);
	}
	else {
		filename = FindDataFile();
	}

	if (filename != NULL && File::Exists(filename)) {
		ResourceManager::Mount(
			RESOURCES_VFS_NAME, filename, nullptr, VFSType::HATCH, VFS_READABLE);
	}
	else {
		VFSMountStatus status = vfs->Mount(RESOURCES_VFS_NAME,
			RESOURCES_DIR_PATH,
			nullptr,
			VFSType::FILESYSTEM,
			VFS_READABLE | VFS_WRITABLE);

		if (status == VFSMountStatus::MOUNTED) {
			Log::Print(Log::LOG_INFO, "Using \"%s\" folder.", RESOURCES_DIR_PATH);

    ResourceManager::LoadModConfig();

    // char modpacksString[1024];
    // if (Application::Settings->GetString("game", "modpacks", modpacksString, sizeof modpacksString)) {
        // if (File::Exists(modpacksString)) {
            // ResourceManager::UsingModPack = true;

            // Log::Print(Log::LOG_IMPORTANT, "Using \"%s\"", modpacksString);
            // ResourceManager::Load(modpacksString);
        // }
    // }
}
void ResourceManager::Load(const char* filename) {
    if (!ResourceRegistry)
        return;
			ResourceManager::UsingDataFolder = true;
		}
		else {
			Log::Print(Log::LOG_ERROR,
				"Could not access \"%s\" folder!",
				RESOURCES_DIR_PATH);
		}
	}

	if (GetMainResource() == nullptr) {
		Log::Print(Log::LOG_ERROR, "No resource files loaded!");

		return false;
	}

	return true;
}
bool ResourceManager::Mount(const char* name,
	const char* filename,
	const char* mountPoint,
	VFSType type,
	Uint16 flags) {
	VFSMountStatus status = vfs->Mount(name, filename, mountPoint, type, flags);

	if (status == VFSMountStatus::NOT_FOUND) {
		Log::Print(Log::LOG_ERROR, "Could not find resource \"%s\"!", filename);
	}
	else if (status != VFSMountStatus::MOUNTED) {
		Log::Print(Log::LOG_ERROR, "Could not load resource \"%s\"!", filename);
	}

	return status == VFSMountStatus::MOUNTED;
}
bool ResourceManager::Unmount(const char* name) {
	if (vfs == nullptr) {
		return false;
	}

	VFSProvider* provider = vfs->Get(name);
	if (provider == nullptr) {
		return false;
	}

	VFSMountStatus status = vfs->Unmount(name);
	if (status == VFSMountStatus::UNMOUNTED) {
		delete provider;

		return true;
	}

	return false;
}

VirtualFileSystem* ResourceManager::GetVFS() {
	return vfs;
}
VFSProvider* ResourceManager::GetMainResource() {
	mainResource = vfs->Get(RESOURCES_VFS_NAME);

	if (mainResource == nullptr || !mainResource->IsOpen()) {
		return nullptr;
	}

	return mainResource;
}
void ResourceManager::SetMainResourceWritable(bool writable) {
	VFSProvider* provider = GetMainResource();
	if (provider != nullptr) {
		provider->SetWritable(writable);
	}
}

bool ResourceManager::LoadResource(const char* filename, Uint8** out, size_t* size) {
	if (vfs) {
		return vfs->LoadFile(filename, out, size);
	}
	return false;
}
bool ResourceManager::ResourceExists(const char* filename) {
	if (vfs) {
		return vfs->FileExists(filename);
	}
	return false;
}
void ResourceManager::Dispose() {
	delete vfs;
	vfs = nullptr;
	mainResource = nullptr;
}

void ResourceManager::LoadModConfig() {
    if (!Directory::Exists("Mods")) {
        Log::Print(Log::LOG_WARN, "No Mods folder");
        return;
    }

    if (ResourceManager::ModConfig)
        ResourceManager::ModConfig->Dispose();

    ResourceManager::ModConfig = INI::Load("Mods/ModConfig.ini");

    if (ResourceManager::ModConfig == nullptr) {
        Log::Print(Log::LOG_WARN, "No ModConfig, creating new ModConfig.ini.");

        ResourceManager::ModConfig = INI::New("Mods/ModConfig.ini");
        ResourceManager::ModConfig->Save("Mods/ModConfig.ini");
    }

    std::vector<char*> modFolders = Directory::GetDirectories("Mods", "*", false);
    for (char* modFolder : modFolders) {
        std::string modName = modFolder;
        modName = modName.substr(modName.find_last_of("/\\") + 1);
        Log::Print(Log::LOG_INFO, "Directory %s", modName.c_str());

        std::string modIniPathStr = std::string(modFolder) + "/Mod.ini";
        const char* modIniPath = modIniPathStr.c_str();

        INI* modIni = INI::Load(modIniPath);
        if (modIni) {
            ModInfo modInfo;
            modInfo.Path = modFolder;
            modInfo.FolderName = modName;

            char name[256], author[256], description[256], version[256];
            modIni->GetString(nullptr, "Name", name, sizeof(name));
            modIni->GetString(nullptr, "Author", author, sizeof(author));
            modIni->GetString(nullptr, "Description", description, sizeof(description));
            modIni->GetString(nullptr, "Version", version, sizeof(version));

            modInfo.Name = name;
            modInfo.Author = author;
            modInfo.Description = description;
            modInfo.Version = version;

            Log::Print(Log::LOG_INFO, "Mod Name: %s", name);
            Log::Print(Log::LOG_INFO, "Author: %s", author);
            Log::Print(Log::LOG_INFO, "Description: %s", description);
            Log::Print(Log::LOG_INFO, "Version: %s", version);

            modIni->Dispose();

            ResourceManager::ModConfig->GetBool("Mods", modName.c_str(), &modInfo.Active);

            ResourceManager::Mods.push_back(modInfo);
        }

        free(modFolder);
    }

    for (const ModInfo& modInfo : ResourceManager::Mods) {
        if (modInfo.Active) {
            std::string modPath = "Mods/" + modInfo.FolderName;
            if (File::Exists((modPath + "/Data.hatch").c_str())) {
                ResourceManager::UsingModPack = true;
                Log::Print(Log::LOG_IMPORTANT, "Using \"%s\"", (modPath + "/Data.hatch").c_str());
            }
            else if (Directory::Exists((modPath + "/Resources").c_str())) {
                ResourceManager::UsingModPack = true;
                Log::Print(Log::LOG_IMPORTANT, "Using \"%s\"", (modPath + "/Resources").c_str());
            }
        }
        else {
            ResourceManager::ModConfig->SetBool("Mods", modInfo.FolderName.c_str(), false);
        }
    }

    ResourceManager::ModConfig->Save("Mods/ModConfig.ini");
}