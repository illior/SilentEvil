// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SECoreTypes.h"
#include "SEGameModeBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPauseClearedSignature);

UCLASS()
class SILENTEVIL_API ASEGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


public:
	ASEGameModeBase();

	FOnPauseClearedSignature OnPauseCleared;

	EGameState GetGameState();

	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
	virtual bool SetInventoryPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause());
	virtual bool ClearPause() override;

protected:
	virtual void BeginPlay() override;

private:
	EGameState CurrentGameState;

};
