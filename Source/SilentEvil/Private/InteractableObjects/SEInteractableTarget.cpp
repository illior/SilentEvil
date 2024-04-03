// Silent Evil Game, All Rights Reserved.

#include "InteractableObjects/SEInteractableTarget.h"

#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"

#include "Player/SEPlayerController.h"
#include "UI/SEGameHUD.h"
#include "UI/SEInventoryContainerWidget.h"
#include "InventorySystem/SEQuestItem.h"

DEFINE_LOG_CATEGORY_STATIC(LogInteractableTarget, All, All);

ASEInteractableTarget::ASEInteractableTarget()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	CameraComponent->SetupAttachment(GetRootComponent());

	DisableAfterInteract = true;
}

void ASEInteractableTarget::StartCanInteract(APawn* Pawn)
{
	Super::StartCanInteract(Pawn);
}

void ASEInteractableTarget::StopCanInteract(APawn* Pawn)
{
	Super::StopCanInteract(Pawn);
}

void ASEInteractableTarget::Interact(APawn* Pawn)
{
	Disable();
	WidgetComponent->SetVisibility(false);

	ASEPlayerController* PlayerController = Cast<ASEPlayerController>(Pawn->GetController());
	if (PlayerController == nullptr)
	{
		return;
	}

	bool OpenWithInventory = InteractableItems.Num() > 0;

	PlayerController->SetInteractTarget(this, OpenWithInventory);

	ReceiveInteract(Pawn);
}

void ASEInteractableTarget::MoveVertical(const float& Value)
{
	ReceiveMoveVertical(Value);
}

void ASEInteractableTarget::MoveHorizontal(const float& Value)
{
	ReceiveMoveHorizontal(Value);
}

void ASEInteractableTarget::Apply()
{
	ReceiveApply();
}

void ASEInteractableTarget::Close()
{
	WidgetComponent->SetVisibility(true);

	ReceiveClose();

	Enable();
	StopCanInteract(CurrentPawn);
}

bool ASEInteractableTarget::TryUseItem(USEBaseItem* InItem)
{
	if (InItem == nullptr || !InItem->IsA<USEQuestItem>())
	{
		return false;
	}
	USEQuestItem* QuestItem = Cast<USEQuestItem>(InItem);

	for (USEBaseItem* Item : InteractableItems)
	{
		if (Item == InItem)
		{
			InteractableItems.Remove(QuestItem);
			ReceiveItemUse(QuestItem);

			return true;
		}
	}

	return false;
}

bool ASEInteractableTarget::GetRemoveItem() const
{
	return RemoveItem;
}

FVector ASEInteractableTarget::GetCameraLocation() const
{
	return CameraComponent->GetComponentLocation();
}

FRotator ASEInteractableTarget::GetCameraRotation() const
{
	return CameraComponent->GetComponentRotation();
}

void ASEInteractableTarget::HideItems()
{
	if (InteractableItems.Num() > 0)
	{
		return;
	}

	if (CurrentPawn == nullptr)
	{
		return;
	}

	APlayerController* PlayerController = CurrentPawn->GetController<APlayerController>();
	if (PlayerController == nullptr)
	{
		return;
	}

	ASEGameHUD* GameHUD = PlayerController->GetHUD<ASEGameHUD>();
	if (GameHUD == nullptr)
	{
		return;
	}

	if(GameHUD->GetInventoryWidget() == nullptr)
	{
		return;
	}

	GameHUD->GetInventoryWidget()->HideItems();
}

void ASEInteractableTarget::Complete()
{
	ASEPlayerController* PlayerController = Cast<ASEPlayerController>(CurrentPawn->GetController());
	if (PlayerController == nullptr)
	{
		return;
	}

	PlayerController->ResumeGame();

	if (!DisableAfterInteract)
	{
		Enable();
	}

	OnComplete.Broadcast(this, CurrentPawn);

	StopCanInteract(CurrentPawn);
}