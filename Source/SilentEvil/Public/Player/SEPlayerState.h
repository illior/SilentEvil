// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SaveSystem/SESavableObject.h"
#include "SEPlayerState.generated.h"

class USEItemData;
struct FSESaveItemData;

UCLASS()
class SILENTEVIL_API ASEPlayerState : public APlayerState, public ISESavableObject
{
	GENERATED_BODY()
	

public:
	virtual FSESaveDataRecord GetSaveDataRecord_Implementation();
	virtual void LoadFromSaveDataRecord_Implementation(FSESaveDataRecord InRecord);

	UFUNCTION(BlueprintCallable, Category = "Storage")
	bool AddItemToStorage(USEItemData* ItemData);

	UFUNCTION(BlueprintCallable, Category = "Storage")
	void RemoveItemFromStorage(USEItemData* ItemData);

	UFUNCTION(BlueprintCallable, Category = "Storage")
	TArray<USEItemData*> GetItemsInStorage();

protected:
	UPROPERTY()
	TArray<USEItemData*> StorageItems;

	UPROPERTY(SaveGame)
	TArray<FSESaveItemData> SaveItems;
};
