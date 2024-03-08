// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SECoreTypes.h"
#include "SECraftingList.generated.h"

UCLASS()
class SILENTEVIL_API USECraftingList : public UDataAsset
{
	GENERATED_BODY()


public:
	USEBaseItem* CanCraft(USEBaseItem* Item1, USEBaseItem* Item2, int32& Count);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemsCraft> Crafts;
};
