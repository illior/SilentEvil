// Silent Evil Game, All Rights Reserved.

#include "UI/MainMenuWidgets/SELoadGameWidget.h"

#include "Kismet/GameplayStatics.h"
#include "SEGameInstance.h"

#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "UI/MainMenuWidgets/SELoadGameItemWidget.h"

void USELoadGameWidget::Open()
{
	PlayAnimationForward(Show);
	SetVisibility(ESlateVisibility::Visible);

	for (USELoadGameItemWidget* Button : SaveSlots)
	{
		Button->UpdateData();
	}
}

void USELoadGameWidget::MoveVertical(int32 Offset)
{
	if (SaveSlots.IsValidIndex(CurrentSlotIndex))
	{
		SaveSlots[CurrentSlotIndex]->SetState(ESEButtonState::Normal);
	}

	CurrentSlotIndex = FMath::Clamp(CurrentSlotIndex - Offset, 0, SaveSlots.Num() - 1);
	SaveSlots[CurrentSlotIndex]->SetState(ESEButtonState::Hovered);
	Scroll->ScrollWidgetIntoView(SaveSlots[CurrentSlotIndex]);
}

void USELoadGameWidget::Apply()
{
	if (SaveSlots.IsValidIndex(CurrentSlotIndex))
	{
		SaveSlots[CurrentSlotIndex]->Click();
	}
}

bool USELoadGameWidget::Cancel()
{
	PlayAnimationForward(Hide);
	return true;
}

void USELoadGameWidget::SetSelected(USELoadGameItemWidget* InButton)
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

void USELoadGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	int32 SavesCount = DefaultSavesCount;
	USEGameInstance* GameInstance = Cast<USEGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance != nullptr)
	{
		SavesCount = GameInstance->GetSavesCount();
	}

	ShowMenuDelegate.BindDynamic(this, &USELoadGameWidget::ActuallyOpenMenu);
	BindToAnimationFinished(Show, ShowMenuDelegate);

	Scroll->ClearChildren();
	SaveSlots.Empty();

	for (int32 i = 0; i < SavesCount; i++)
	{
		USELoadGameItemWidget* SaveSlotWidget = CreateWidget<USELoadGameItemWidget>(GetWorld(), LoadGameItemWidgetClass);
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

void USELoadGameWidget::ActuallyOpenMenu()
{

}
