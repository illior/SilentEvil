// Silent Evil Game, All Rights Reserved.

#include "Player/SEPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "UI/SEGameHUD.h"

#include "SEGameModeBase.h"
#include "InteractableObjects/SEInteractableTarget.h"

DEFINE_LOG_CATEGORY_STATIC(LogSEPlayerController, All, All);

void ASEPlayerController::ResumeGame()
{
	if (GetWorld() == nullptr || GetWorld()->GetAuthGameMode() == nullptr)
	{
		return;
	}

	ASEGameHUD* SEHUD = Cast<ASEGameHUD>(MyHUD);
	if (SEHUD == nullptr)
	{
		return;
	}

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
	SEHUD->Close();

	GetWorld()->GetAuthGameMode()->ClearPause();
	if (GetViewTarget() != GetPawn())
	{
		SetViewTargetWithBlend(GetPawn(), BlendTime);
	}

	SetGameMappingContext();
	UE_LOG(LogSEPlayerController, Display, TEXT("Resume game"));
}

void ASEPlayerController::SetInteractTarget(ASEInteractableTarget* Target, bool ShowItems)
{
	if (Target == nullptr || GetPawn() == nullptr)
	{
		return;
	}

	SetViewTargetWithBlend(Target, BlendTime);

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("OpenTargetMenu"), ShowItems);
	GetWorldTimerManager().SetTimer(InvokeTimerHandle, TimerDelegate, BlendTime + 0.05f, false);
}

void ASEPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(OpenMapAction, ETriggerEvent::Started, this, &ASEPlayerController::OpenMap);
		EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &ASEPlayerController::OpenInventory);
		EnhancedInputComponent->BindAction(OpenRecordsAction, ETriggerEvent::Started, this, &ASEPlayerController::OpenRecords);
		EnhancedInputComponent->BindAction(OpenMenuAction, ETriggerEvent::Started, this, &ASEPlayerController::OpenMenu);
	}
}

void ASEPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetGameMappingContext();
}

void ASEPlayerController::PauseGame()
{
	SetInputMode(FInputModeGameAndUI());
	bShowMouseCursor = true;
	PlayerCameraManager->StopAllCameraShakes();
}

void ASEPlayerController::OpenTargetMenu(bool ShowItems)
{
	if (GetWorld() == nullptr || GetWorld()->GetAuthGameMode() == nullptr)
	{
		return;
	}

	ASEGameModeBase* GameMode = Cast<ASEGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		return;
	}

	if (!GameMode->SetInventoryPause(this))
	{
		return;
	}

	ASEGameHUD* SEHUD = Cast<ASEGameHUD>(MyHUD);
	if (SEHUD == nullptr)
	{
		return;
	}
	SEHUD->OpenTargetInventory(ShowItems);

	SetMappingContext(InventoryMenuMappingContext);
	UE_LOG(LogSEPlayerController, Display, TEXT("Pause Target"));

	PauseGame();

	ASEInteractableTarget* ViewTarget = Cast<ASEInteractableTarget>(GetViewTarget());
	FVector NewPosition = ViewTarget->GetCameraLocation();
	NewPosition.Z = GetPawn()->GetActorLocation().Z;
	GetPawn()->SetActorLocation(NewPosition);

	SetControlRotation(ViewTarget->GetCameraRotation());
}

void ASEPlayerController::OpenMap()
{
	if (GetWorld() == nullptr || GetWorld()->GetAuthGameMode() == nullptr)
	{
		return;
	}

	ASEGameModeBase* GameMode = Cast<ASEGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		return;
	}

	if (!GameMode->SetInventoryPause(this))
	{
		return;
	}

	ASEGameHUD* SEHUD = Cast<ASEGameHUD>(MyHUD);
	if (SEHUD == nullptr)
	{
		return;
	}
	SEHUD->OpenMap();

	SetMappingContext(InventoryMenuMappingContext);
	UE_LOG(LogSEPlayerController, Display, TEXT("Pause Inventory"));

	PauseGame();
}

void ASEPlayerController::OpenInventory()
{
	if (GetWorld() == nullptr || GetWorld()->GetAuthGameMode() == nullptr)
	{
		return;
	}

	ASEGameModeBase* GameMode = Cast<ASEGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		return;
	}

	if (!GameMode->SetInventoryPause(this))
	{
		return;
	}

	ASEGameHUD* SEHUD = Cast<ASEGameHUD>(MyHUD);
	if (SEHUD == nullptr)
	{
		return;
	}
	SEHUD->OpenInventory();

	SetMappingContext(InventoryMenuMappingContext);
	UE_LOG(LogSEPlayerController, Display, TEXT("Pause Inventory"));

	PauseGame();
}

void ASEPlayerController::OpenRecords()
{
	if (GetWorld() == nullptr || GetWorld()->GetAuthGameMode() == nullptr)
	{
		return;
	}

	ASEGameModeBase* GameMode = Cast<ASEGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		return;
	}

	if (!GameMode->SetInventoryPause(this))
	{
		return;
	}

	ASEGameHUD* SEHUD = Cast<ASEGameHUD>(MyHUD);
	if (SEHUD == nullptr)
	{
		return;
	}
	SEHUD->OpenRecords();

	SetMappingContext(InventoryMenuMappingContext);
	UE_LOG(LogSEPlayerController, Display, TEXT("Pause Inventory"));

	PauseGame();
}

void ASEPlayerController::OpenMenu()
{
	if (GetWorld() == nullptr || GetWorld()->GetAuthGameMode() == nullptr)
	{
		return;
	}

	ASEGameModeBase* GameMode = Cast<ASEGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		return;
	}

	if (!GameMode->SetPause(this))
	{
		return;
	}

	ASEGameHUD* SEHUD = Cast<ASEGameHUD>(MyHUD);
	if (SEHUD == nullptr)
	{
		return;
	}
	SEHUD->OpenMenu();

	SetMappingContext(PauseMenuMappingContext);
	UE_LOG(LogSEPlayerController, Display, TEXT("Pause Menu"));

	PauseGame();
}

void ASEPlayerController::SetGameMappingContext()
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem == nullptr)
	{
		return;
	}

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(LocomotionMappingContext, 0);
	Subsystem->AddMappingContext(OpenUIMappingContext, 1);
}

void ASEPlayerController::SetMappingContext(UInputMappingContext* NewMappingContext)
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem == nullptr)
	{
		return;
	}

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(NewMappingContext, 0);
}
