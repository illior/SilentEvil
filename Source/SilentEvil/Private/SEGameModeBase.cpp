// Silent Evil Game, All Rights Reserved.


#include "SEGameModeBase.h"

#include "Player/SECharacter.h"
#include "Player/SEPlayerController.h"
#include "UI/SEGameHUD.h"
#include "Player/SEPlayerState.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/SESaveGame.h"
#include "SaveSystem/SESavableObject.h"

DEFINE_LOG_CATEGORY_STATIC(LogSEGameMode, All, All);

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

void ASEGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	if (SelectedSaveSlot.Len() > 0)
	{
		UE_LOG(LogSEGameMode, Display, TEXT("Try load game from slot %s."), *SelectedSaveSlot);
		LoadGameFromSlot(SelectedSaveSlot);
	}
}

void ASEGameModeBase::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);

	if (UGameplayStatics::ParseOption(OptionsString, "SaveGame").Len() == 0)
	{
		return;
	}

	APawn* Pawn = NewPlayer->GetPawn();
	if (Pawn != nullptr && Pawn->Implements<USESavableObject>())
	{
		ISESavableObject::Execute_LoadFromSaveDataRecord(Pawn, CharacterData);
	}

	APlayerState* PlayerState = NewPlayer->PlayerState;
	if (PlayerState != nullptr && PlayerState->Implements<USESavableObject>())
	{
		ISESavableObject::Execute_LoadFromSaveDataRecord(PlayerState, PlayerStateData);
	}
}

USESaveGame* ASEGameModeBase::SaveGameToSlot(FString SlotName)
{
	UE_LOG(LogSEGameMode, Display, TEXT("Save game to slot %s."), *SlotName);

	DataRecords.Empty();
	SaveActors();

	TArray<uint8> BinaryData;
	FMemoryWriter Writer = FMemoryWriter(BinaryData);
	FObjectAndNameAsStringProxyArchive Ar(Writer, false);
	Ar.ArIsSaveGame = true;

	Serialize(Ar);

	USESaveGame* SaveInstance = Cast<USESaveGame>(UGameplayStatics::CreateSaveGameObject(USESaveGame::StaticClass()));
	SaveInstance->ByteData = BinaryData;
	SaveInstance->SaveTime = UKismetMathLibrary::Now();
	SaveInstance->MapName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	UGameplayStatics::SaveGameToSlot(SaveInstance, SlotName, 0);

	return SaveInstance;
}

void ASEGameModeBase::LoadGameFromSlot(FString SlotName)
{
	DataRecords.Empty();

	USESaveGame* LoadGameInstance = Cast<USESaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (LoadGameInstance == nullptr)
	{
		UE_LOG(LogSEGameMode, Display, TEXT("Failed to load game."));
		return;
	}

	UE_LOG(LogSEGameMode, Display, TEXT("Load game from slot %s."), *SlotName);
	TArray<uint8> BinaryData = LoadGameInstance->ByteData;
	FMemoryReader Reader = FMemoryReader(BinaryData);
	FObjectAndNameAsStringProxyArchive Ar(Reader, false);
	Ar.ArIsSaveGame = true;

	Serialize(Ar);

	TArray<AActor*> FindActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USESavableObject::StaticClass(), FindActors);

	for (AActor* Actor : FindActors)
	{
		FString ActorName = Actor->GetName();

		for (int32 i = 0; i < DataRecords.Num(); i++)
		{
			if (DataRecords[i].ActorName == ActorName)
			{
				ISESavableObject::Execute_LoadFromSaveDataRecord(Actor, DataRecords[i]);

				break;
			}
		}
	}

	DataRecords.Empty();
}

void ASEGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentGameState = EGameState::InProgress;
}

void ASEGameModeBase::SaveActors()
{
	TArray<AActor*> FindActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USESavableObject::StaticClass(), FindActors);
	for (AActor* Actor : FindActors)
	{
		if (Actor->GetClass()->IsChildOf<ASECharacter>())
		{
			CharacterData = ISESavableObject::Execute_GetSaveDataRecord(Actor);
			continue;
		}

		if (Actor->GetClass()->IsChildOf<ASEPlayerState>())
		{
			PlayerStateData = ISESavableObject::Execute_GetSaveDataRecord(Actor);
			continue;
		}

		DataRecords.Add(ISESavableObject::Execute_GetSaveDataRecord(Actor));
	}
}
