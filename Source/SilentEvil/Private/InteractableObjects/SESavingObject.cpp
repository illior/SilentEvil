// Silent Evil Game, All Rights Reserved.

#include "InteractableObjects/SESavingObject.h"

#include "Kismet/GameplayStatics.h"
#include "UI/SESubmenuWidget.h"
#include "Components/WidgetComponent.h"
#include "Player/SEPlayerController.h"

void ASESavingObject::Interact(APawn* Pawn)
{
	Disable();
	WidgetComponent->SetVisibility(false);

	ASEPlayerController* PlayerController = Cast<ASEPlayerController>(Pawn->GetController());
	if (PlayerController == nullptr)
	{
		return;
	}

	if (Widget == nullptr)
	{
		FindOrCreateWidget();
	}

	PlayerController->SetInteractTarget(this, Widget);

	ReceiveInteract(Pawn);
}

USESubmenuWidget* ASESavingObject::GetWidget()
{
	return Widget;
}

void ASESavingObject::SetWidget(USESubmenuWidget* InWidget)
{
	Widget = InWidget;
}

void ASESavingObject::FindOrCreateWidget()
{
	TArray<AActor*> FindActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), StaticClass(), FindActors);

	for (AActor* Actor : FindActors)
	{
		ASESavingObject* SavingObject = Cast<ASESavingObject>(Actor);

		if (SavingObject->GetWidget() != nullptr)
		{
			Widget = SavingObject->GetWidget();
			return;
		}
	}

	Widget = CreateWidget<USESubmenuWidget>(GetWorld(), WidgetClass);

	for (AActor* Actor : FindActors)
	{
		ASESavingObject* SavingObject = Cast<ASESavingObject>(Actor);

		SavingObject->SetWidget(Widget);
	}
}
