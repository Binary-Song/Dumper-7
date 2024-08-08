#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include "UnrealObjects.h"

namespace fs = std::filesystem;

class ObjectArray
{
private:
	friend struct FChunkedFixedUObjectArray;
	friend struct FFixedUObjectArray;
	friend class ObjectArrayValidator;

private:
	static uint8* GObjects;
	static uint32 NumElementsPerChunk;
	static uint32 SizeOfFUObjectItem;
	static uint32 FUObjectItemInitialOffset;

public:
	static std::string DecryptionLambdaStr;

private:
	static inline void*(*ByIndex)(void* ObjectsArray, int32 Index, uint32 FUObjectItemSize, uint32 FUObjectItemOffset, uint32 PerChunk) = nullptr;

	static inline uint8_t* (*DecryptPtr)(void* ObjPtr) = [](void* Ptr) -> uint8* { return (uint8*)Ptr; };

private:
	static void InitializeFUObjectItem(uint8_t* FirstItemPtr);
	static void InitializeChunkSize(uint8_t* GObjects);

public:
	static void InitDecryption(uint8_t* (*DecryptionFunction)(void* ObjPtr), const char* DecryptionLambdaAsStr);

	static void Init(bool bScanAllMemory = false);

	static void Init(int32 GObjectsOffset, int32 NumElementsPerChunk, bool bIsChunked);

	static void DumpObjects(const fs::path& Path, bool bWithPathname = false);

	static int32 Num();

	template<typename UEType = UEObject>
	static UEType GetByIndex(int32 Index);

	template<typename UEType = UEObject>
	static UEType FindObject(std::string FullName, EClassCastFlags RequiredType = EClassCastFlags::None);

	template<typename UEType = UEObject>
	static UEType FindObjectFast(std::string Name, EClassCastFlags RequiredType = EClassCastFlags::None);

	template<typename UEType = UEObject>
	static UEType FindObjectFastInOuter(std::string Name, std::string Outer);

	static UEClass FindClass(std::string FullName);

	static UEClass FindClassFast(std::string Name);

	class ObjectsIterator
	{
		ObjectArray& IteratedArray;
		UEObject CurrentObject;
		int32 CurrentIndex;

	public:
		ObjectsIterator(ObjectArray& Array, int32 StartIndex = 0);

		UEObject operator*();
		ObjectsIterator& operator++();
		bool operator!=(const ObjectsIterator& Other);

		int32 GetIndex() const;
	};

	ObjectsIterator begin();
	ObjectsIterator end();

	static inline void* DEBUGGetGObjects()
	{
		return GObjects;
	}
};


#define InitObjectArrayDecryption(DecryptionLambda) ObjectArray::InitDecryption(DecryptionLambda, #DecryptionLambda)

template<typename UEType>
inline UEType ObjectArray::GetByIndex(int32 Index)
{
	return UEType(ByIndex(GObjects + Off::FUObjectArray::Ptr, Index, SizeOfFUObjectItem, FUObjectItemInitialOffset, NumElementsPerChunk));
}

template<typename UEType>
inline UEType ObjectArray::FindObject(std::string FullName, EClassCastFlags RequiredType)
{
	for (UEObject Object : ObjectArray())
	{
		if (Object.IsA(RequiredType) && Object.GetFullName() == FullName)
		{
			return Object.Cast<UEType>();
		}
	}

	return UEType();
}

template<typename UEType>
inline UEType ObjectArray::FindObjectFast(std::string Name, EClassCastFlags RequiredType)
{
	auto ObjArray = ObjectArray();

	for (UEObject Object : ObjArray)
	{
		if (Object.IsA(RequiredType) && Object.GetName() == Name)
		{
			return Object.Cast<UEType>();
		}
	}

	return UEType();
}

template<typename UEType>
inline UEType ObjectArray::FindObjectFastInOuter(std::string Name, std::string Outer)
{
	auto ObjArray = ObjectArray();

	for (UEObject Object : ObjArray)
	{
		if (Object.GetName() == Name && Object.GetOuter().GetName() == Outer)
		{
			return Object.Cast<UEType>();
		}
	}

	return UEType();
}
