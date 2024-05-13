// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SECoreTypes.h"
#include "SEGameModeBase.generated.h"

class USESaveGame;

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

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);

	virtual void RestartPlayer(AController* NewPlayer);

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	USESaveGame* SaveGameToSlot(FString SlotName);

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void LoadGameFromSlot(FString SlotName);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(SaveGame)
	TArray<FSESaveDataRecord> DataRecords;

	UPROPERTY(SaveGame)
	FSESaveDataRecord CharacterData;
	UPROPERTY(SaveGame)
	FSESaveDataRecord PlayerStateData;

	EGameState CurrentGameState;

	void SaveActors();
};
