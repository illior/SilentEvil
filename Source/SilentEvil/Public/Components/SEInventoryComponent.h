// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SECoreTypes.h"
#include "SEInventoryComponent.generated.h"

class USEItemData;
class USEWeaponData;
class USECraftingList;

struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemCraftedSignature, USEItemData*, CreatedItem);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SILENTEVIL_API USEInventoryComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	USEInventoryComponent();

	FOnItemCraftedSignature OnItemCrafted;

	FIntPoint GetMaxSize() const;
	int32 GetCurrentSlotsCount() const;

	TIndexedContainerIterator<const TArray<USEItemData*, FDefaultAllocator>, USEItemData* const, int32> GetItemsIterator() const;

	ESlotState GetSlotState(const int32& X, const int32& Y) const;
	ESlotState GetSlotState(const FIntPoint& Position) const;
	ESlotState GetSlotState(const int32& Index) const;

	USEItemData* GetItemDataByPosition(const int32& X, const int32& Y);
	bool MoveItemTo(USEItemData* ItemData, const int32& X, const int32& Y);
	void UseItem(USEItemData* ItemData);
	bool DropItem(USEItemData* ItemData);

	bool TryAddItem(USEBaseItem* Item, int32& Count);

	void ReloadEquipWeapon();

	void SetEquipWeapon(USEItemData* ItemData);
	USEWeaponData* GetEquipWeapon() const;

	void SetFastAccessWeapon(USEItemData* ItemData, int32 Index);
	USEWeaponData* GetFastAccessWeapon(int32 Index) const;

	int32 GetCountAmmoType(EAmmoType AmmoType);

	void FastAccessItem(const FInputActionValue& Value);

protected:
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadWrite, Category = "Inventory")
	TArray<USEItemData*> Items;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	FIntPoint MaxSize = FIntPoint(4, 5);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	int32 CurrentSlotsCount = 8;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InventoryCrafting")
	USECraftingList* CraftingList = nullptr;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	TMap<EAmmoType, int32> Ammunation;

	UPROPERTY()
	TArray<USEWeaponData*> FastAccessWeapons;

	UPROPERTY()
	USEWeaponData* CurrentWeapon = nullptr;

	ESlotState** ItemsMatrix;

	void Initialize();

	USEItemData* CreateItemData(USEBaseItem* Item, const int32& X, const int32& Y, const int32& Count = 1);
	void DestroyItem(USEItemData* ItemData);

	bool MoveShortItem(USEItemData* ItemData, const int32& X, const int32& Y);
	bool MoveLongItem(USEItemData* ItemData, const int32& X, const int32& Y);

	void SetSlotState(const int32& X, const int32& Y, ESlotState State);
	void SetItemPosition(USEItemData* Item, const int32& X, const int32& Y);

	bool TryCraft(USEItemData* ItemData1, USEItemData* ItemData2);
	bool TryCombine(USEItemData* ItemData1, USEItemData* ItemData2);

	void TryAddAmmo(USEItemData* ItemData);
	void RemoveAmmo(EAmmoType AmmoType, int32 Count);

	void CreateMatrix();
	void DestroyMatrix();

	void LogInfo();
};
