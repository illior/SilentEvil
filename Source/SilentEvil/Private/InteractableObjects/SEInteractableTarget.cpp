// Silent Evil Game, All Rights Reserved.

#include "InteractableObjects/SEInteractableTarget.h"

#include "Camera/CameraComponent.h"

#include "Player/SEPlayerController.h"
#include "InventorySystem/SEBaseItem.h"

DEFINE_LOG_CATEGORY_STATIC(LogInteractableTarget, All, All);

ASEInteractableTarget::ASEInteractableTarget()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	CameraComponent->SetupAttachment(GetRootComponent());

	DisableAfterInteract = true;
}

void ASEInteractableTarget::Interact(APawn* Pawn)
{
	ASEPlayerController* PlayerController = Cast<ASEPlayerController>(Pawn->GetController());
	if (PlayerController == nullptr)
	{
		return;
	}

	CurrentPawn = Pawn;
	PlayerController->SetInteractTarget(this, WithInventory);

	ReceiveInteract(Pawn);
}

void ASEInteractableTarget::Complete()
{
	ASEPlayerController* PlayerController = Cast<ASEPlayerController>(CurrentPawn->GetController());
	if (PlayerController == nullptr)
	{
		return;
	}

	PlayerController->ResumeGame();

	if (DisableAfterInteract)
	{
		Disable();
	}
	
	OnComplete.Broadcast(this, CurrentPawn);
	CurrentPawn = nullptr;
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
	ReceiveClose();
}

bool ASEInteractableTarget::TryUseItem(USEBaseItem* InItem)
{
	for (USEBaseItem* Item : InteractableItems)
	{
		if (Item == InItem)
		{
			InteractableItems.Remove(InItem);
			ReceiveItemUse(InItem);

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
