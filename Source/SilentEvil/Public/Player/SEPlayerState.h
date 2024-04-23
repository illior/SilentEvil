// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SEPlayerState.generated.h"

class USEItemData;

UCLASS()
class SILENTEVIL_API ASEPlayerState : public APlayerState
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable, Category = "Storage")
	bool AddItemToStorage(USEItemData* ItemData);

	UFUNCTION(BlueprintCallable, Category = "Storage")
	void RemoveItemFromStorage(USEItemData* ItemData);

	UFUNCTION(BlueprintCallable, Category = "Storage")
	TArray<USEItemData*> GetItemsInStorage();

protected:
	UPROPERTY()
	TArray<USEItemData*> StorageItems;
};
