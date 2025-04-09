#ifdef ENABLE_ANGELSCRIPT

#include "Engine/Bytecode/ScriptManager.h"
#include "Engine/ResourceTypes/ResourceManager.h"
#include "Engine/Hashing/CombinedHash.h"

#include <angelscript.h>
#include "add_on/scriptbuilder/scriptbuilder.h"

//ScriptManager -> Script engine
//VMThread -> Script Context

namespace ScriptManager {

asIScriptEngine *engine;

void RequestGarbageCollection(){
    ForceGarbageCollection();
}

void ForceGarbageCollection(){
    engine->GarbageCollect();
}

void ResetStack(){

}

void Init(){
    engine = asCreateScriptEngine();
}

void DisposeGlobalValueTable(HashMap<VMValue>* globals){

}

void Dispose(){
    engine->ShutDownAndRelease();
}

void FreeString(ObjString* string){
	if (string->Chars != NULL) {
		Memory::Free(string->Chars);
	}
	string->Chars = NULL;

    Memory::Free(string);
}

void FreeGlobalValue(Uint32 hash, VMValue value){

}

VMValue CastValueAsString(VMValue v, bool prettyPrint){

}

VMValue CastValueAsString(VMValue v){

}

VMValue CastValueAsInteger(VMValue v){

}

VMValue CastValueAsDecimal(VMValue v){

}

VMValue Concatenate(VMValue va, VMValue vb){

}

bool ValuesSortaEqual(VMValue a, VMValue b){

}

bool ValuesEqual(VMValue a, VMValue b){

}

bool ValueFalsey(VMValue a){

}

VMValue DelinkValue(VMValue val){

}

bool DoIntegerConversion(VMValue& value, Uint32 threadID){

}

bool DoDecimalConversion(VMValue& value, Uint32 threadID){

}

void FreeValue(VMValue value){

}

bool Lock(){

}

void Unlock(){

}

void DefineMethod(VMThread* thread, ObjFunction* function, Uint32 hash){

}

void DefineNative(ObjClass* klass, const char* name, NativeFn function){
    engine->RegisterGlobalFunction(name, asFUNCTION(function), asCALL_CDECL);
}

void GlobalLinkInteger(ObjClass* klass, const char* name, int* value){

}

void GlobalLinkDecimal(ObjClass* klass, const char* name, float* value){

}

void GlobalConstInteger(ObjClass* klass, const char* name, int value){

}

void GlobalConstDecimal(ObjClass* klass, const char* name, float value){

}

ObjClass* GetClassParent(ObjClass* klass){

}

VMValue GetClassMethod(ObjClass* klass, Uint32 hash){

}

void LinkStandardLibrary(){

}

void LinkExtensions(){

}

bool RunBytecode(BytecodeContainer bytecodeContainer, Uint32 filenameHash){

}

bool CallFunction(char* functionName){

}

Entity* SpawnObject(const char* objectName){

}

Uint32 MakeFilenameHash(const char* filename){
	size_t length = strlen(filename);
	const char* dot = strrchr(filename, '.');
	if (dot) {
		length = dot - filename;
	}
	return CombinedHash::EncryptData((const void*)filename, length);
}

std::string GetBytecodeFilenameForHash(Uint32 filenameHash){
	char filename[sizeof(OBJECTS_DIR_NAME) + 12];
	snprintf(filename, sizeof filename, "%s%08X.ibc", OBJECTS_DIR_NAME, filenameHash);
	return std::string(filename);
}

BytecodeContainer GetBytecodeFromFilenameHash(Uint32 filenameHash){
	if (Sources->Exists(filenameHash)) {
		return Sources->Get(filenameHash);
	}

	BytecodeContainer bytecode;
	bytecode.Data = nullptr;
	bytecode.Size = 0;

	std::string filenameForHash = GetBytecodeFilenameForHash(filenameHash);
	const char* filename = filenameForHash.c_str();

	if (!ResourceManager::ResourceExists(filename)) {
		return bytecode;
	}

	ResourceStream* stream = ResourceStream::New(filename);
	if (!stream) {
		// Object doesn't exist?
		return bytecode;
	}

	bytecode.Size = stream->Length();
	bytecode.Data = (Uint8*)Memory::TrackedMalloc("Bytecode::Data", bytecode.Size);
	stream->ReadBytes(bytecode.Data, bytecode.Size);
	stream->Close();

	Sources->Put(filenameHash, bytecode);

	return bytecode;
}

bool ClassExists(const char* objectName){

}

bool IsStandardLibraryClass(const char* className){

}

bool LoadScript(char* filename){
	if (!filename || !*filename) {
		return false;
	}

	Uint32 hash = MakeFilenameHash(filename);
	return LoadScript(hash);
}

bool LoadScript(const char* filename){
	if (!filename || !*filename) {
		return false;
	}

	Uint32 hash = MakeFilenameHash(filename);
	return LoadScript(hash);
}

bool LoadScript(Uint32 hash){
	if (!Sources->Exists(hash)) {
		BytecodeContainer bytecode = GetBytecodeFromFilenameHash(hash);
		if (!bytecode.Data) {
			return false;
		}

		return RunBytecode(bytecode, hash);
	}

	return true;
}

bool LoadObjectClass(const char* objectName, bool addNativeFunctions){

}

void AddNativeObjectFunctions(ObjClass* klass){

}

ObjClass* GetObjectClass(const char* className){

}

Entity* ObjectSpawnFunction(ObjectList* list){

}

void LoadClasses(){
	SourceFileMap::ClassMap->ForAll([](Uint32, vector<Uint32>* filenameHashList) -> void {
		for (size_t fn = 0; fn < filenameHashList->size(); fn++) {
			Uint32 filenameHash = (*filenameHashList)[fn];

			BytecodeContainer bytecode =
				ScriptManager::GetBytecodeFromFilenameHash(filenameHash);
			if (!bytecode.Data) {
				Log::Print(
					Log::LOG_WARN, "Class %08X does not exist!", filenameHash);
				continue;
			}

			RunBytecode(bytecode, filenameHash);
		}
	});

	ScriptManager::ForceGarbageCollection();
}

} //end namespace ScriptManager

#endif
