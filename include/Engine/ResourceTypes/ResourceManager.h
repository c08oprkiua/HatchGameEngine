#ifndef ENGINE_RESOURCETYPES_RESOURCEMANAGER_H
#define ENGINE_RESOURCETYPES_RESOURCEMANAGER_H

#include <Engine/Filesystem/VFS/VFSProvider.h>
#include <Engine/Filesystem/VFS/VirtualFileSystem.h>
#include <Engine/Includes/Standard.h>
#include <Engine/Includes/HashMap.h>
#include <Engine/TextFormats/INI/INI.h>

struct ModInfo {
    std::string Path;
    std::string ID;
    std::string Name;
    std::string Description;
    std::string Author;
    std::string Version;
    std::string FolderName;
    bool Active;
};

class ResourceManager {
public:
	static bool                 UsingDataFolder;
	static bool                 UsingModPack;
    static std::vector<ModInfo> Mods;
    static INI*                 ModConfig;

	static bool Init(const char* filename);
	static bool Mount(const char* name,
		const char* filename,
		const char* mountPoint,
		VFSType type,
		Uint16 flags);
	static bool Unmount(const char* name);
	static VirtualFileSystem* GetVFS();
	static VFSProvider* GetMainResource();
	static void SetMainResourceWritable(bool writable);
	static bool LoadResource(const char* filename, Uint8** out, size_t* size);
	static bool ResourceExists(const char* filename);
	static void Dispose();
    static void LoadModConfig();
};

#endif /* ENGINE_RESOURCETYPES_RESOURCEMANAGER_H */