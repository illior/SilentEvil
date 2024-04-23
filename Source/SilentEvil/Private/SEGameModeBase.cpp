// Silent Evil Game, All Rights Reserved.


#include "SEGameModeBase.h"

#include "Player/SECharacter.h"
#include "Player/SEPlayerController.h"
#include "UI/SEGameHUD.h"
#include "Player/SEPlayerState.h"

ASEGameModeBase::ASEGameModeBase()
{
	DefaultPawnClass = ASECharacter::StaticClass();
	PlayerControllerClass = ASEPlayerController::StaticClass();
	HUDClass = ASEGameHUD::StaticClass();
	PlayerStateClass = ASEPlayerState::StaticClass();
}

EGameState ASEGameModeBase::GetGameState()
{
	return CurrentGameState;
}

bool ASEGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	if (CurrentGameState == EGameState::InInventroy)
	{
		return false;
	}

	const bool PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
	if (PauseSet)
	{
		CurrentGameState = EGameState::InPause;
	}

	return PauseSet;
}

bool ASEGameModeBase::SetInventoryPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	if (CurrentGameState == EGameState::InPause)
	{
		return false;
	}

	const bool PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
	if (PauseSet)
	{
		CurrentGameState = EGameState::InInventroy;
	}

	return PauseSet;
}

bool ASEGameModeBase::ClearPause()
{
	const bool PauseSet = Super::ClearPause();
	if (PauseSet)
	{
		CurrentGameState = EGameState::InProgress;
		OnPauseCleared.Broadcast();
	}

	return PauseSet;
}

void ASEGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentGameState = EGameState::InProgress;
}
