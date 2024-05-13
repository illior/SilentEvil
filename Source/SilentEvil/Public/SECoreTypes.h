#pragma once

#include "SECoreTypes.generated.h"

class USEBaseItem;

USTRUCT(BlueprintType)
struct FItemsCraft
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USEBaseItem* Item1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USEBaseItem* Item2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USEBaseItem* Result = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	int32 Count = 1;

	bool Check(USEBaseItem* CheckItem1, USEBaseItem* CheckItem2)
	{
		return (Item1 == CheckItem1 && Item2 == CheckItem2) || (Item1 == CheckItem2 && Item2 == CheckItem1);
	}
};

UENUM(BlueprintType)
enum class ESEInventoryOpenState : uint8 {
	Base		UMETA(DisplayName = "Base"),
	Interaction	UMETA(DisplayName = "Interaction"),
	Storage		UMETA(DisplayName = "Storage"),
};

UENUM(BlueprintType)
enum class ESEInventoryState : uint8 {
	Selected,
	DropDownMenu,
	ItemMoving,
	ListenBind,
	ItemInspect,
	Storage
};

UENUM(BlueprintType)
enum class ESlotState : uint8 {
	Void	UMETA(DisplayName = "Void"),
	Item	UMETA(DisplayName = "Item"),
	Block	UMETA(DisplayName = "Block"),
};

UENUM(BlueprintType)
enum class EGameState : uint8
{
	InProgress	UMETA(DisplayName = "InProgress"),
	InPause		UMETA(DisplayName = "InPause"),
	InInventroy	UMETA(DisplayName = "InInventroy")
};

UENUM(BlueprintType)
enum class ESEButtonState : uint8
{
	Normal		UMETA(DisplayName = "Normal"),
	Hovered		UMETA(DisplayName = "Hover"),
	Pressed		UMETA(DisplayName = "Pressed"),
	Disabled	UMETA(DisplayName = "Disable")
};

UENUM(BlueprintType)
enum class EViewState : uint8
{
	Normal		UMETA(DisplayName = "Normal"),
	Selected	UMETA(DisplayName = "Hover"),
	Active		UMETA(DisplayName = "Disable")
};

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	Pistol	UMETA(DisplayName = "Pistol"),
	Shotgun	UMETA(DisplayName = "Shotgun")
};

USTRUCT(BlueprintType)
struct FSESaveDataRecord
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame)
	UClass* ActorClass;

	UPROPERTY(SaveGame)
	FString ActorName;

	UPROPERTY(SaveGame)
	FTransform ActorTransform;

	UPROPERTY(SaveGame)
	TArray<uint8> BinaryData;
};

USTRUCT(BlueprintType)
struct FSESaveDataComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame)
	FString ComponentName;

	UPROPERTY(SaveGame)
	FTransform ComponentTransform;

	UPROPERTY(SaveGame)
	TArray<uint8> BinaryData;
};