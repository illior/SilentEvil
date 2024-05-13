// Silent Evil Game, All Rights Reserved.

#include "UI/PauseMenuWidgets/SESaveGameWidget.h"

#include "Kismet/GameplayStatics.h"
#include "SEGameInstance.h"

#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "UI/PauseMenuWidgets/SESaveGameItemWidget.h"

#include "Player/SEPlayerController.h"
#include "InteractableObjects/SEInteractableTarget.h"

void USESaveGameWidget::Open()
{
	Super::Open();
}

void USESaveGameWidget::Close()
{
	ASEPlayerController* Controller = GetOwningPlayer<ASEPlayerController>();
	if (Controller != nullptr)
	{
		ASEInteractableTarget* Target = Cast<ASEInteractableTarget>(Controller->GetViewTarget());
		if(Target != nullptr)
		{
			Target->Close();
		}

		Controller->ResumeGame();
	}

	Super::Close();
}

void USESaveGameWidget::MoveVertical(int32 Offset)
{
	if (SaveSlots.IsValidIndex(CurrentSlotIndex))
	{
		SaveSlots[CurrentSlotIndex]->SetState(ESEButtonState::Normal);
	}

	CurrentSlotIndex = FMath::Clamp(CurrentSlotIndex - Offset, 0, SaveSlots.Num() - 1);
	SaveSlots[CurrentSlotIndex]->SetState(ESEButtonState::Hovered);
	Scroll->ScrollWidgetIntoView(SaveSlots[CurrentSlotIndex]);
}

void USESaveGameWidget::Apply()
{
	if (SaveSlots.IsValidIndex(CurrentSlotIndex))
	{
		SaveSlots[CurrentSlotIndex]->Click();
	}
}

bool USESaveGameWidget::Cancel()
{
	return true;
}

void USESaveGameWidget::SetSelected(USESaveGameItemWidget* InButton)
{
	if (SaveSlots.IsValidIndex(CurrentSlotIndex))
	{
		SaveSlots[CurrentSlotIndex]->SetState(ESEButtonState::Normal);
	}

	if (SaveSlots.Contains(InButton))
	{
		CurrentSlotIndex = SaveSlots.Find(InButton);
	}
}

void USESaveGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	int32 SavesCount = DefaultSavesCount;
	USEGameInstance* GameInstance = Cast<USEGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance != nullptr)
	{
		SavesCount = GameInstance->GetSavesCount();
	}

	Scroll->ClearChildren();
	SaveSlots.Empty();

	for (int32 i = 0; i < SavesCount; i++)
	{
		USESaveGameItemWidget* SaveSlotWidget = CreateWidget<USESaveGameItemWidget>(GetWorld(), SaveGameItemWidgetClass);
		if (SaveSlotWidget == nullptr)
		{
			continue;
		}

		SaveSlotWidget->SetSlotName(FString::Printf(TEXT("%i"), i + 1));
		SaveSlotWidget->MenuWidget = this;

		UPanelSlot* PanelSlot = Scroll->AddChild(SaveSlotWidget);
		if (PanelSlot != nullptr && PanelSlot->IsA<UScrollBoxSlot>())
		{
			UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(PanelSlot);
			ScrollBoxSlot->SetPadding(SaveSlotPadding);
		}

		SaveSlots.Add(SaveSlotWidget);
	}
}
