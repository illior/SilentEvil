// Silent Evil Game, All Rights Reserved.

#include "InteractableObjects/SEStorageBox.h"

#include "Components/WidgetComponent.h"
#include "Player/SEPlayerController.h"

ASEStorageBox::ASEStorageBox()
{
	DisableAfterInteract = false;
	RemoveItem = true;
}

void ASEStorageBox::Interact(APawn* Pawn)
{
	Disable();
	WidgetComponent->SetVisibility(false);

	ASEPlayerController* PlayerController = Pawn->GetController<ASEPlayerController>();
	if (PlayerController == nullptr)
	{
		return;
	}

	PlayerController->SetInteractTarget(this, true);

	ReceiveInteract(Pawn);
}

void ASEStorageBox::MoveVertical(const float& Value)
{
	Super::MoveVertical(Value);
}

void ASEStorageBox::MoveHorizontal(const float& Value)
{
	Super::MoveHorizontal(Value);
}

void ASEStorageBox::Apply()
{
	Super::Apply();
}

void ASEStorageBox::Close()
{
	Super::Close();
}
