#include <Windows.h>
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

/*
	guttir14 https://github.com/guttir14/CheatIt
*/

struct FNameEntryHandle {
	uint32_t Block = 0;
	uint32_t Offset = 0;

	FNameEntryHandle(uint32_t block, uint32_t offset) : Block(block), Offset(offset) {};
	FNameEntryHandle(uint32_t id) : Block(id >> 16), Offset(id & 65535) {};
	operator uint32_t() const { return (Block << 16 | Offset); }
};

struct FNameEntry {
	uint32_t comparison_id;
	uint16_t bIsWide : 1;
	uint16_t Len : 15;
	union
	{
		char AnsiName[1024];
		wchar_t	WideName[1024];
	};

	std::string String();
};

struct FNamePool
{
	BYTE Lock[8];
	uint32_t CurrentBlock;
	uint32_t CurrentByteCursor;
	BYTE* Blocks[8192];

	FNameEntry* GetEntry(FNameEntryHandle handle) const;
};

struct FName {
	uint32_t comparison_index; // 0x0
	uint32_t Index; // 0x04
	uint32_t Number; // 0x08

	std::string GetName();
};


struct UObject {
	void** VFTable; // 0x00
	uint32_t ObjectFlags; // 0x08 
	uint32_t InternalIndex; // 0x0C
	struct UClass* ClassPrivate; // 0x10 
	FName NamePrivate; // 0x18
	char pad_000[0x4]; 
	UObject* OuterPrivate; // 0x28

	std::string GetName();
	std::string GetFullName();
	bool IsA(void* cmp);
	void ProcessEvent(void* fn, void* parms);
};

// Class CoreUObject.Field
// Size: 0x30 (Inherited: 0x28)
struct UField : UObject {
	char pad_30[0x8]; // 0x30(0x08)
};

// Class CoreUObject.Struct
// Size: 0xb0 (Inherited: 0x30)
struct UStruct : UField {
	char pad_30[0x10]; // 0x38(0x10)
	UStruct* SuperStruct; // 0x48(0x8)
	char UnknownData_50[0x68]; // 0x50(0x80)
};

// Class CoreUObject.Class
// Size: 0x230 (Inherited: 0xb0)
struct UClass : UStruct {
	char pad_B8[0x188]; // 0xb8(0x188)
};

struct TUObjectArray {
	BYTE** Objects;
	BYTE* PreAllocatedObjects;
	uint32_t MaxElements;
	uint32_t NumElements;
	uint32_t MaxChunks;
	uint32_t NumChunks;

	UObject* GetObjectPtr(uint32_t id) const;
	UObject* FindObject(const char* name) const;
};

template<class T>
struct TArray
{
	friend struct FString;

public:
	inline int Num() const
	{
		return Count;
	};

	inline T& operator[](int i)
	{
		return Data[i];
	};

	inline bool IsValidIndex(int i) const
	{
		return i < Num();
	}

private:
	T* Data;
	int32_t Count;
	int32_t Max;
};

struct FString : private TArray<wchar_t>
{
	inline FString()
	{
	};

	FString(const wchar_t* other)
	{
		Max = Count = *other ? std::wcslen(other) + 1 : 0;

		if (Count)
		{
			Data = const_cast<wchar_t*>(other);
		}
	};

	inline bool IsValid() const
	{
		return Data != nullptr;
	}

	inline const wchar_t* c_str() const
	{
		return Data;
	}

	std::string ToString() const
	{
		auto length = std::wcslen(Data);

		std::string str(length, '\0');

		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

		return str;
	}
};

struct FLinearColor
{
	float R;
	float G;
	float B;
	float A;
};

struct FMatrix {
	float M[4][4];
};

struct FVector2D
{
	float X;
	float Y;
};

struct FVector {
	float X;
	float Y;
	float Z;
};

struct FRotator {
	float Pitch;
	float Yaw;
	float Roll;
};

// Class Engine.Actor 
struct AActor : UObject {
	char pad_0000[0xF8]; // 0x30 (0x30) 
	class APawn* Instigator; // 0x128(0x08)
	char pad_0002[0x10]; // 0x130 (0x10)
	class USceneComponent* RootComponent; // 0x140 (0x08)

	float GetDistanceTo(AActor* OtherActor);
};

// Class Engine.Controller 
struct AController : AActor {
	char pad_0000[0xF0]; // 0x148 (0xF0)
	class APlayerState* PlayerState; // 0x238(0x08)
	bool LineOfSightTo(AActor* Other);
	void SetControlRotation(FRotator NewRotation);
};

// Class Engine.PlayerController
struct APlayerController : AController {
	char pad_0000[0x70]; // 0x240 (0x68)
	class UPlayer* Player; // 0x2b0(0x08)
	class APawn* AcknowledgedPawn; // 0x2B8 (0x08)
	bool ProjectWorldLocationToScreen(FVector& WorldLocation, FVector2D& ScreenLocation);
	void GetViewportSize(INT& X, INT& Y);
};

// Class Engine.Pawn 
struct APawn : AActor {
	char pad_0000[0x108]; // 0x148 (0x108)
	class APlayerState* PlayerState; // 0x250(0x08)
	char pad_0001[0x10]; // 0x258 (0x10)
	class AController* Controller; // 0x268(0x08)
	char pad_0002[0x28];// 0x270 (0x20)
	class USkeletalMeshComponent* Mesh; // 0x298 (0x08) 

	int8_t GetPlayerTeam();
};

// Class DeadByDaylight.Generator
struct AGenerator : AActor
{
	char pad_0000[0x1F1]; // 0x148 (0x108)
	bool IsRepaired; // 0x339(0x01)
};

// Class DeadByDaylight.Locker
// Size: 0x420 (Inherited: 0x328)
struct ALocker : AActor {
	char pad_000[0x0];
};

// Class DeadByDaylight.MeatHook
// Size: 0x560 (Inherited: 0x328)
struct AMeatHook : AActor {
	char pad_000[0x0];
};

// Class DeadByDaylight.Searchable
// Size: 0x3e8 (Inherited: 0x328)
struct ASearchable : AActor {
	char pad_0000[0x0]; // 0x148 (0x108)
};

// Class DeadByDaylight.EscapeDoor
// Size: 0x420 (Inherited: 0x328)
struct AEscapeDoor : AActor {
	char pad_0000[0x0]; // 0x148 (0x108)
};

// Class DeadByDaylight.Totem
// Size: 0x450 (Inherited: 0x328)
struct ATotem : AActor {
	char pad_0000[0x220]; // 0x148 (0x108)
	int8_t _totemState; // 0x368(0x01)
};

// Class Engine.Level
struct ULevel {
	char pad_0000[0xA0]; // 0x00 (0x98)
	TArray<AActor*> Actors; // 0xA0 (0x10)
	char pad_0001[0x10]; // 0xB0 (0x10)
	class UWorld* OwningWorld; // 0xC0 (0x08)
};

// Class Engine.GameInstance
struct UGameInstance {
	char pad_0000[0x40]; // 0x0 (0x38)
	TArray<class UPlayer*> LocalPlayers; // 0x40(0x10)
};

// Class Engine.Player
struct UPlayer {
	char pad_0000[0x38]; // 0x0 (0x30)
	class APlayerController* PlayerController; // 0x38(0x08)
	char pad_0001[0x38]; // 0x40 (0x38)
	class UGameViewportClient* ViewportClient; // 0x78 (0x08)
};

// Class Engine.PlayerState
struct APlayerState {
	char pad_0000[0x298]; // 0x0 (0x280)
	class APawn* PawnPrivate; // 0x298 (0x08)
	char pad_0001[0x78]; // 0x2A0 (0x78)
	FString PlayerNamePrivate; // 0x318 (0x10)
};

// Class Engine.SkinnedMeshComponent
struct USkeletalMeshComponent : UObject {
	char pad_0000[0x00]; // 0x28
	FName GetBoneName(INT BoneIndex);
	FVector GetBoneMatrix(INT index);
};

// Class Engine.SceneComponent
struct USceneComponent {
	char pad_0000[0x134]; // 0x0 (0x11C)
	struct FVector RelativeLocation; // 0x134(0x0C)
	struct FRotator RelativeRotation; // 0x140(0x0C)
};

// Class Engine.GameStateBase
struct AGameStateBase
{
	char pad_0000[0x248];
	TArray<class APlayerState*> PlayerArray; // 0x248(0x10)
};

// Class Engine.World 
struct UWorld {
	char pad_0000[0x38]; // 0x0 (0x30)
	class ULevel* PersistentLevel; // 0x38(0x08)
	char pad_0001[0xF0]; // 0x40 (0x148)
	AGameStateBase* GameState; // 0x130(0x08)
	char pad_0002[0x58]; // 0x138 (0x148)
	class UGameInstance* OwningGameInstance; // 0x190(0x08)
};

// Class Engine.Canvas
struct Canvas : UObject {
	char pad_0000[0x00]; // 0x28
	void K2_DrawLine(FVector2D ScreenPositionA, FVector2D ScreenPositionB, FLOAT Thickness, FLinearColor Color);
	void K2_DrawText(FString RenderText, FVector2D ScreenPosition, FVector2D Scale, FLinearColor RenderColor, float Kerning, FLinearColor ShadowColor, FVector2D ShadowOffset, bool bCentreX, bool bCentreY, bool bOutlined, FLinearColor OutlineColor);
};

// Class Engine.GameViewportClient
struct UGameViewportClient : UObject {
	char pad_0000[0x00]; // 0x28 
};

enum BoneFNames {
	Char,
	Head,
	Neck,
	TorsoA,
	TorsoB,
	TorsoC,
	HipMaster,
	HipRT,
	KneeRT,
	FootRT,
	HipLT,
	KneeLT,
	FootLT,
	ClavicleLT,
	ShoulderLT,
	ElbowLT,
	HandLT,
	ClavicleRT,
	ShoulderRT,
	ElbowRT,
	HandRT,
};

extern FNamePool* NamePoolData;
extern TUObjectArray* ObjObjects;
extern UWorld* WRLD;
extern UObject* WorldToScreenUFunc;
extern UObject* GetViewportSizeUFunc;
extern UObject* GetBoneNameUFunc;
extern UObject* K2_DrawLineUFunc;
extern UObject* K2_DrawTextUFunc;
extern UObject* SetControlRotationUFunc;
extern UObject* LineOfSightToUFunc;
extern UObject* EnemyClass;
extern UObject* GetPlayerTeamUFunc;
extern UObject* AGeneratorClass;
extern UObject* GetDistanceToUFunc;
extern UObject* AEscapeDoorClass;
extern UObject* HookClass;
extern UObject* ASearchableClass;
extern UObject* ATotemClass;
extern uintptr_t GetBoneMatrixF;
extern void(*OPostRender)(UGameViewportClient* UGameViewportClient, Canvas* Canvas);

bool EngineInit();
FVector2D GetBone(USkeletalMeshComponent* Mesh, INT index, APlayerController* PlayerController);
void GetAllBoneNames(USkeletalMeshComponent* Mesh);
int GetBoneIndex(USkeletalMeshComponent* Mesh, BoneFNames Bone);
