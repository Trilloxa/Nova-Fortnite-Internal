#pragma once
#include "../../stdafx.h"

struct FNameEntryHandle {
	uint32_t Block = 0;
	uint32_t Offset = 0;

	FNameEntryHandle(uint32_t block, uint32_t offset) : Block(block), Offset(offset) {};
	FNameEntryHandle(uint32_t id) : Block(id >> 16), Offset(id & 65535) {};
	operator uint32_t() const { return (Block << 16 | Offset); }
};

struct FNameEntryHeader
{
	uint16_t bIsWide : 1;
	static constexpr uint32_t ProbeHashBits = 5;
	uint16_t LowercaseProbeHash : ProbeHashBits;
	uint16_t Len : 10;
};

struct FNameEntry {
	FNameEntryHeader Header;

	union
	{
		char AnsiName[1024];
		wchar_t	WideName[1024];
	};

	inline int32_t GetId() const
	{
		return *(uint16_t*)&Header;
	}
};

std::string FName::GetName()
{
	std::string name(Utils::GetObjectNameByIndex(Index));
	if (Number > 0)
	{
		name += '_' + std::to_string(Number);
	}
	auto pos = name.rfind('/');
	if (pos != std::string::npos)
	{
		name = name.substr(pos + 1);
	}
	return name;
}

struct FNameEntryAllocator
{
	unsigned char frwLock[0x8];
	int32_t CurrentBlock;
	int32_t CurrentByteCursor;
	uint8_t* Blocks[8192];

	inline int32_t NumBlocks() const
	{
		return CurrentBlock + 1;
	}
	inline FNameEntry* GetById(int32_t key) const
	{
		int block = key >> 16;
		int offset = (uint16_t)key;

		if (!IsValidIndex(key, block, offset))
			return reinterpret_cast<FNameEntry*>(Blocks[0] + 0); // "None"

		return reinterpret_cast<FNameEntry*>(Blocks[block] + ((uint64_t)offset * 0x02));
	}
	inline bool IsValidIndex(int32_t key) const
	{
		uint32_t block = key >> 16;
		uint16_t offset = key;
		return IsValidIndex(key, block, offset);
	}
	inline bool IsValidIndex(int32_t key, uint32_t block, uint16_t offset) const
	{
		return (key >= 0 && block < NumBlocks() && offset * 0x02 < 0x1FFFE);
	}
};

struct FNamePool
{
	FNameEntryAllocator Allocator;
	uint32_t AnsiCount;
	uint32_t WideCount;
};

struct UObject {
	void** VFTable;
	uint32_t ObjectFlags;
	uint32_t InternalIndex;
	UObject* ClassPrivate;
	FName NamePrivate;
	UObject* OuterPrivate;

	std::string GetName()
	{
		return NamePrivate.GetName();
	}

	std::string GetFullName()
	{
		std::string name;
		for (auto outer = OuterPrivate; outer; outer = outer->OuterPrivate) { name = outer->GetName() + "." + name; }
		name = ClassPrivate->GetName() + " " + name + this->GetName();
		return name;
	}
};

struct UField : UObject {
	char UnknownData_28[0x8]; // 0x28(0x08)
};

struct UStruct : UField {
	char pad_30[0x10]; // 0x30(0x10)
	UStruct* SuperStruct; // 0x40(0x8)
	char UnknownData_48[0x68]; // 0x48(0x80)
};

class UScriptStruct : public UStruct
{
public:
	uint8                                        Pad_AC[0x10];                                      // Fixing Size Of Struct [ Dumper-7 ]

};

template<typename KeyType, typename ValueType>
class TPair
{
public:
	KeyType                                                    First;                                                   // 0x0000(0x0000)
	ValueType                                                  Second;                                                  // 0x0000(0x0000)
};

template<typename ElementType>
class TSetElement
{
public:
	ElementType                                                Value;                                                   // 0x0000(0x0000)
	int32_t                                                    HashNextId;                                              // 0x0000(0x0000)
	int32_t                                                    HashIndex;                                               // 0x0000(0x0000)
};

template<typename Key, typename Value>
class TMap
{
public:
	TArray<TSetElement<TPair<Key, Value>>>                     Data;                                                    // 0x0000(0x0000)
private:
	uint8_t                                                    UnknownData01[0x04];                                     // 0x0000(0x0000)
	uint8_t                                                    UnknownData02[0x04];                                     // 0x0000(0x0000)
	uint8_t                                                    UnknownData03[0x08];                                     // 0x0000(0x0000)
	uint8_t                                                    UnknownData04[0x08];                                     // 0x0000(0x0000)
	uint8_t                                                    UnknownData_MayBeSize[0x04];                             // 0x0000(0x0000)
	uint8_t                                                    UnknownData_MayBeFlag[0x04];                             // 0x0000(0x0000)
	uint8_t                                                    UnknownData05[0x08];                                     // 0x0000(0x0000)
	uint8_t                                                    UnknownData06[0x08];                                     // 0x0000(0x0000)
	uint8_t                                                    UnknownData07[0x08];                                     // 0x0000(0x0000)
	uint8_t                                                    UnknownData_MayBeSize02[0x04];                           // 0x0000(0x0000)
	uint8_t                                                    UnknownData08[0x04];                                     // 0x0000(0x0000)
};

class UDataTable : public UObject
{
public:
	UScriptStruct* RowStruct;                                         // 0x28(0x8)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	TMap<FName, uint8_t*>                                      RowMap;                                     // Fixing Size After Last Property  [ Dumper-7 ]
	uint8                                        bStripFromClientBuilds : 1;                        // Mask : 0x1 0x80(0x1)(Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                        Pad_40C[0x1F];                                     // Fixing Size Of Struct [ Dumper-7 ]


public:
	inline uintptr_t FindRow(const FName& rowName) const
	{
		for (int32_t i = 0; i < RowMap.Data.Num(); ++i)
		{
			TPair<FName, unsigned char*> curRowPair = RowMap.Data[i].Value;
			FName& curRowName = curRowPair.First;
			if (curRowName.Index == rowName.Index && curRowName.Number == rowName.Number)
			{
				return reinterpret_cast<uintptr_t>(curRowPair.Second);

			}
		}

		return 0;
	}
};

struct FDataTableRowHandle
{
public:
	UDataTable* DataTable;                                         // 0x0(0x8)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	FName                                  RowName;                                           // 0x8(0x8)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

// Class CoreUObject.Function
struct UFunction : UStruct {
	char pad_B0[0x30]; // 0xb0(0x30)
};

// Class CoreUObject.Class
struct UClass : UStruct {
	char UnknownData_B0[0x180]; // 0xb0(0x180)
};

struct TUObjectArray {
	BYTE** Objects;
	BYTE* PreAllocatedObjects;
	uint32_t MaxElements;
	uint32_t NumElements;
	uint32_t MaxChunks;
	uint32_t NumChunks;

	UObject* GetObjectPtr(uint32_t id) const
	{
		if (id >= NumElements) return nullptr;
		uint64_t chunkIndex = id / 65536;
		if (chunkIndex >= NumChunks) return nullptr;
		auto chunk = Objects[chunkIndex];
		if (!chunk) return nullptr;
		uint32_t withinChunkIndex = id % 65536 * 24;
		auto item = *reinterpret_cast<UObject**>(chunk + withinChunkIndex);
		return item;
	}

	UObject* FindObject(const char* name) const
	{
		for (auto i = 0u; i < NumElements; i++)
		{
			auto object = GetObjectPtr(i);
			if (object && object->GetFullName() == name) { return object; }
		}
		return nullptr;
	}

};

TUObjectArray* GObjects;