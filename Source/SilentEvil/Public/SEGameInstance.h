// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SEGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SILENTEVIL_API USEGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	int32 GetSavesCount();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SaveGame", meta = (ClampMin = "1", ClampMax = "20"))
	int32 SavesCount = 20;
};
