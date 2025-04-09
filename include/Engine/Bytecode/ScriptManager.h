#ifndef ENGINE_BYTECODE_SCRIPTMANAGER_H
#define ENGINE_BYTECODE_SCRIPTMANAGER_H

class ScriptEntity;

#include <Engine/Bytecode/Types.h>
#include <Engine/Bytecode/VMThread.h>
#include <Engine/IO/MemoryStream.h>
#include <Engine/IO/ResourceStream.h>
#include <Engine/IO/Stream.h>
#include <Engine/Includes/Standard.h>
#include <Engine/Types/Entity.h>
#include <set>

#define OBJECTS_DIR_NAME "Objects/"

namespace ScriptManager {
//private:
	Uint32 GetBranchLimit();
	void RemoveNonGlobalableValue(Uint32 hash, VMValue value);
	void FreeNativeValue(Uint32 hash, VMValue value);
	void FreeFunction(ObjFunction* function);
	void FreeModule(ObjModule* module);
	void FreeClass(ObjClass* klass);
	void FreeEnumeration(ObjEnum* enumeration);
	void FreeNamespace(ObjNamespace* ns);
	void FreeModules();

//public:
	extern bool LoadAllClasses;
	extern HashMap<VMValue>* Globals;
	extern HashMap<VMValue>* Constants;
	extern std::set<Obj*> FreedGlobals;
	extern VMThread Threads[8];
	extern Uint32 ThreadCount;
	extern vector<ObjModule*> ModuleList;
	extern HashMap<BytecodeContainer>* Sources;
	extern HashMap<ObjClass*>* Classes;
	extern HashMap<char*>* Tokens;
	extern vector<ObjNamespace*> AllNamespaces;
	extern vector<ObjClass*> ClassImplList;
	extern SDL_mutex* GlobalLock;

	void RequestGarbageCollection();
	void ForceGarbageCollection();
	void ResetStack();

	void Init();
	void Dispose();

	void FreeString(ObjString* string);
	void FreeGlobalValue(Uint32 hash, VMValue value);
	void DisposeGlobalValueTable(HashMap<VMValue>* globals);

	VMValue CastValueAsString(VMValue v, bool prettyPrint);
	VMValue CastValueAsString(VMValue v);
	VMValue CastValueAsInteger(VMValue v);
	VMValue CastValueAsDecimal(VMValue v);
	VMValue Concatenate(VMValue va, VMValue vb);
	bool ValuesSortaEqual(VMValue a, VMValue b);
	bool ValuesEqual(const VMValue &a, const VMValue &b);
	bool ValueFalsey(VMValue a);
	VMValue DelinkValue(VMValue val);
	bool DoIntegerConversion(VMValue& value, Uint32 threadID);
	bool DoDecimalConversion(VMValue& value, Uint32 threadID);
	void FreeValue(VMValue value);

	bool Lock();
	void Unlock();

	void DefineMethod(VMThread* thread, ObjFunction* function, Uint32 hash);
	void DefineNative(ObjClass* klass, const char* name, NativeFn function);


	void GlobalLinkInteger(ObjClass* klass, const char* name, int* value);
	void GlobalLinkDecimal(ObjClass* klass, const char* name, float* value);
	void GlobalConstInteger(ObjClass* klass, const char* name, int value);
	void GlobalConstDecimal(ObjClass* klass, const char* name, float value);

	ObjClass* GetClassParent(ObjClass* klass);
	VMValue GetClassMethod(ObjClass* klass, Uint32 hash);
	bool ClassExists(const char* objectName);
	bool IsStandardLibraryClass(const char* className);

	void LinkStandardLibrary();
	void LinkExtensions();

	bool RunBytecode(BytecodeContainer bytecodeContainer, Uint32 filenameHash);
	bool CallFunction(char* functionName);
	Entity* SpawnObject(const char* objectName);

	Uint32 MakeFilenameHash(const char* filename);
	std::string GetBytecodeFilenameForHash(Uint32 filenameHash);
	BytecodeContainer GetBytecodeFromFilenameHash(Uint32 filenameHash);

	bool LoadScript(char* filename);
	bool LoadScript(const char* filename);
	bool LoadScript(Uint32 hash);

	bool LoadObjectClass(const char* objectName, bool addNativeFunctions);

	void AddNativeObjectFunctions(ObjClass* klass);
	ObjClass* GetObjectClass(const char* className);
	Entity* ObjectSpawnFunction(ObjectList* list);
	void LoadClasses();
}

#endif /* ENGINE_BYTECODE_SCRIPTMANAGER_H */
