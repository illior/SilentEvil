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

	
	//penTargetMenu(ShowItems);
	
	SetViewTarget(Target);
	//SetViewTargetWithBlend(Target, BlendTime);

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("OpenTargetMenu"), ShowItems);
	GetWorldTimerManager().SetTimer(InvokeTimerHandle, TimerDelegate, 0.1f, false);
}

void ASEPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &ASEPlayerController::OpenInventory);
		EnhancedInputComponent->BindAction(OpenMenuAction, ETriggerEvent::Started, this, &ASEPlayerController::OpenMenu);
	}
}

void ASEPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetGameMappingContext();
}

void ASEPlayerController::SetPause()
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

	SetPause();

	FVector NewPosition = Cast<ASEInteractableTarget>(GetViewTarget())->GetCameraLocation();
	NewPosition.Z = GetPawn()->GetActorLocation().Z;
	GetPawn()->SetActorLocation(NewPosition);
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

	SetPause();
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

	SetPause();
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
