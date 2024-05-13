// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SESaveGame.generated.h"

UCLASS()
class SILENTEVIL_API USESaveGame : public USaveGame
{
	GENERATED_BODY()
	

public:
	UPROPERTY()
	TArray<uint8> ByteData;

	UPROPERTY(BlueprintReadOnly)
	FDateTime SaveTime;

	UPROPERTY(BlueprintReadOnly)
	FString MapName;
};
