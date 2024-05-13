// Silent Evil Game, All Rights Reserved.

#include "UI/MainMenuWidgets/SELoadGameItemWidget.h"

#include "UI/MainMenuWidgets/SELoadGameWidget.h"
#include "Components/TextBlock.h"

#include "Kismet/GameplayStatics.h"
#include "SaveSystem/SESaveGame.h"

void USELoadGameItemWidget::SetSlotName(FString InName)
{
	SlotName = InName;

	Text = FText::FromString(InName);
	ButtonText->SetText(FText::FromString(InName));

	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		SaveDate->SetText(NoDataMessage);

		return;
	}
	

	USESaveGame* LoadGameInstance = Cast<USESaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (LoadGameInstance != nullptr)
	{
		SaveDate->SetText(FText::FromString(LoadGameInstance->SaveTime.ToString(TEXT("%d.%m.%y-%H:%M"))));
	}
}

FString USELoadGameItemWidget::GetSlotName()
{
	return SlotName;
}

void USELoadGameItemWidget::UpdateData()
{
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		SaveDate->SetText(NoDataMessage);

		return;
	}


	USESaveGame* LoadGameInstance = Cast<USESaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (LoadGameInstance != nullptr)
	{
		SaveDate->SetText(FText::FromString(LoadGameInstance->SaveTime.ToString(TEXT("%d.%m.%y-%H:%M"))));
	}
}

void USELoadGameItemWidget::Click()
{
	OnClicked.Broadcast();
}

void USELoadGameItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OnClicked.AddDynamic(this, &USELoadGameItemWidget::OpenLevel);
}

void USELoadGameItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (MenuWidget != nullptr)
	{
		MenuWidget->SetSelected(this);
	}

	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void USELoadGameItemWidget::OpenLevel()
{
	USESaveGame* LoadGameInstance = Cast<USESaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (LoadGameInstance == nullptr)
	{
		return;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName(LoadGameInstance->MapName), true, FString::Printf(TEXT("?savegame=%s"), *SlotName));
}
