// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SECoreTypes.h"
#include "SEInventoryComponent.generated.h"

class USEItemData;
class USEWeaponData;
class USECraftingList;
class UDataTable;

struct FSEReadableRecord;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAddSignature, USEItemData*, NewItem);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SILENTEVIL_API USEInventoryComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	USEInventoryComponent();

	FOnItemAddSignature OnItemAdd;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FIntPoint GetMaxSize() const;
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetCurrentSlotsCount() const;

	TIndexedContainerIterator<const TArray<USEItemData*, FDefaultAllocator>, USEItemData* const, int32> GetItemsIterator() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	ESlotState GetSlotState(const int32& X, const int32& Y) const;
	ESlotState GetSlotState(const FIntPoint& Position) const;
	ESlotState GetSlotState(const int32& Index) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	USEItemData* GetItemDataByPosition(const int32& X, const int32& Y);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool MoveItemTo(USEItemData* ItemData, const int32& X, const int32& Y);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UseItem(USEItemData* ItemData);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool DropItem(USEItemData* ItemData, bool IsGarbage = true);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TryAddItem(USEBaseItem* Item, int32& Count);
	bool TryAddItem(USEItemData* ItemData);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddRecord(FName RowName);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FSEReadableRecord> GetRecords();

	void ReloadEquipWeapon();

	void SetEquipWeapon(USEItemData* ItemData);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	USEWeaponData* GetEquipWeapon() const;

	void SetFastAccessWeapon(USEItemData* ItemData, int32 Index);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	USEWeaponData* GetFastAccessWeapon(int32 Index) const;

	int32 GetCountAmmoType(EAmmoType AmmoType);

	void FastAccessItem(const int32 index);

protected:
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadWrite, Category = "Inventory")
	TArray<USEItemData*> Items;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	FIntPoint MaxSize = FIntPoint(4, 5);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	int32 CurrentSlotsCount = 8;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InventoryCrafting")
	USECraftingList* CraftingList = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory") // , meta = (RequiredAssetDataTags = "RowStructure=FSEReadableRecord")
	UDataTable* RecordsTable;

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

	UPROPERTY()
	TArray<FName> Records;

	void Initialize();

	USEItemData* CreateItemData(USEBaseItem* Item, const int32& X, const int32& Y, const int32& Count = 1);
	void AddItemData(USEItemData* ItemData);
	void DestroyItem(USEItemData* ItemData, bool IsGarbage = true);

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
