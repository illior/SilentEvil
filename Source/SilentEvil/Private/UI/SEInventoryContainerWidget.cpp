// Silent Evil Game, All Rights Reserved.


#include "UI/SEInventoryContainerWidget.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Components/HorizontalBox.h"
#include "UI/SEInventoryWidget.h"

#include "Player/SEPlayerController.h"
#include "SEGameModeBase.h"
#include "InteractableObjects/SEInteractableTarget.h"

DEFINE_LOG_CATEGORY_STATIC(LogSEInventoryContainer, All, All);

void USEInventoryContainerWidget::Open(bool ForInteraction, bool ShowItems)
{
	if (InventoryWidget == nullptr)
	{
		return;
	}

	SetVisibility(ESlateVisibility::Visible);
	InventoryWidget->UpdateInventoryData(ForInteraction);

	CanChangeWidget = !ForInteraction;
	if (ForInteraction)
	{
		TopMenu->SetVisibility(ESlateVisibility::Hidden);

		ASEPlayerController* CharacterController = Cast<ASEPlayerController>(GetOwningPlayer());

		if (CharacterController != nullptr)
		{
			TargetObject = Cast<ASEInteractableTarget>(CharacterController->GetViewTarget());
		}

		CurrentIndex = 1;
		ShowItems ? InventoryWidget->SetVisibility(ESlateVisibility::Visible) : InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		TopMenu->SetVisibility(ESlateVisibility::Visible);
		CurrentIndex = 1;
		Widgets[CurrentIndex]->SetVisibility(ESlateVisibility::Visible);
	}
}

void USEInventoryContainerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Widgets.Add(MapWidget);
	Widgets.Add(InventoryWidget);
	Widgets.Add(RecordsWidget);

	SetupInput();
}

void USEInventoryContainerWidget::SetupInput()
{
	ASEPlayerController* Controller = GetOwningPlayer<ASEPlayerController>();
	if (Controller == nullptr)
	{
		return;
	}

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(Controller->InputComponent);
	if (EnhancedInputComponent == nullptr)
	{
		return;
	}

	EnhancedInputComponent->BindAction(MoveVerticalAction, ETriggerEvent::Triggered, this, &USEInventoryContainerWidget::MoveVertical);
	EnhancedInputComponent->BindAction(MoveHorizontalAction, ETriggerEvent::Triggered, this, &USEInventoryContainerWidget::MoveHorizontal);

	EnhancedInputComponent->BindAction(AdditiveHorizontalAction, ETriggerEvent::Triggered, this, &USEInventoryContainerWidget::AdditiveHorizontal);

	EnhancedInputComponent->BindAction(MoveItemAction, ETriggerEvent::Started, this, &USEInventoryContainerWidget::MoveItem);
	EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Started, this, &USEInventoryContainerWidget::Cancel);
	EnhancedInputComponent->BindAction(ApplyAction, ETriggerEvent::Started, this, &USEInventoryContainerWidget::Apply);

	EnhancedInputComponent->BindAction(FastAccessAction, ETriggerEvent::Started, this, &USEInventoryContainerWidget::FastAccessItem);
}

void USEInventoryContainerWidget::AdditiveHorizontal(const FInputActionValue& Value)
{
	if (ESlateVisibility::Collapsed == GetVisibility())
	{
		return;
	}

	if (CanChangeWidget && Widgets[CurrentIndex] != nullptr && Widgets[CurrentIndex]->GetVisibility() == ESlateVisibility::Visible)
	{
		int32 Offset = (int32)Value.Get<float>();

		if (CurrentIndex + Offset >= 0 && CurrentIndex + Offset < Widgets.Num())
		{
			Widgets[CurrentIndex]->SetVisibility(ESlateVisibility::Collapsed);
			CurrentIndex += Offset;
			Widgets[CurrentIndex]->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{

	}
}

void USEInventoryContainerWidget::Cancel()
{
	if (ESlateVisibility::Collapsed == GetVisibility())
	{
		return;
	}

	bool CanClose = true;
	if (Widgets[CurrentIndex] != nullptr && Widgets[CurrentIndex]->GetVisibility() == ESlateVisibility::Visible)
	{
		CanClose = Widgets[CurrentIndex]->Cancel();
	}

	if (TargetObject != nullptr)
	{
		TargetObject->Close();
	}
	
	if (CanClose)
	{
		for (USEBaseWidget* Widget : Widgets)
		{
			Widget->SetVisibility(ESlateVisibility::Collapsed);
		}

		ASEPlayerController* CharacterController = Cast<ASEPlayerController>(GetOwningPlayer());
		if (CharacterController != nullptr)
		{
			TargetObject = nullptr;
			CharacterController->ResumeGame();
		}
	}
}

void USEInventoryContainerWidget::MoveItem()
{
	if (ESlateVisibility::Collapsed == GetVisibility())
	{
		return;
	}

	if (Widgets[CurrentIndex] != nullptr && Widgets[CurrentIndex]->GetVisibility() == ESlateVisibility::Visible)
	{
		Widgets[CurrentIndex]->MoveItem();
	}
}

void USEInventoryContainerWidget::MoveVertical(const FInputActionValue& Value)
{
	if (ESlateVisibility::Collapsed == GetVisibility())
	{
		return;
	}

	if (Widgets[CurrentIndex] != nullptr && Widgets[CurrentIndex]->GetVisibility() == ESlateVisibility::Visible)
	{
		Widgets[CurrentIndex]->MoveVertical(Value.Get<float>());
		return;
	}

	if (TargetObject != nullptr)
	{
		TargetObject->MoveVertical(Value.Get<float>());
	}
}

void USEInventoryContainerWidget::MoveHorizontal(const FInputActionValue& Value)
{
	if (ESlateVisibility::Collapsed == GetVisibility())
	{
		return;
	}

	if (Widgets[CurrentIndex] != nullptr && Widgets[CurrentIndex]->GetVisibility() == ESlateVisibility::Visible)
	{
		Widgets[CurrentIndex]->MoveHorizontal(Value.Get<float>());
		return;
	}

	if (TargetObject != nullptr)
	{
		TargetObject->MoveHorizontal(Value.Get<float>());
	}
}

void USEInventoryContainerWidget::FastAccessItem(const FInputActionValue& Value)
{
	if (ESlateVisibility::Collapsed == GetVisibility())
	{
		return;
	}

	if (Widgets[CurrentIndex] != nullptr && Widgets[CurrentIndex]->GetVisibility() == ESlateVisibility::Visible)
	{
		Widgets[CurrentIndex]->FastAccessItem(Value.Get<float>());
	}
}

void USEInventoryContainerWidget::Apply()
{
	if (ESlateVisibility::Collapsed == GetVisibility())
	{
		return;
	}

	if (Widgets[CurrentIndex] != nullptr && Widgets[CurrentIndex]->GetVisibility() == ESlateVisibility::Visible)
	{
		Widgets[CurrentIndex]->Apply();
		return;
	}

	if (TargetObject != nullptr)
	{
		TargetObject->Apply();
	}
}
