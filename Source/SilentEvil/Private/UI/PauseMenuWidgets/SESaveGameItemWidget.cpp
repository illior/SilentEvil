// Silent Evil Game, All Rights Reserved.


#include "UI/PauseMenuWidgets/SESaveGameItemWidget.h"

#include "UI/PauseMenuWidgets/SESaveGameWidget.h"
#include "Components/TextBlock.h"

#include "Kismet/GameplayStatics.h"
#include "SaveSystem/SESaveGame.h"
#include "SEGameModeBase.h"

void USESaveGameItemWidget::SetSlotName(FString InName)
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

FString USESaveGameItemWidget::GetSlotName()
{
	return SlotName;
}

void USESaveGameItemWidget::Click()
{
	OnClicked.Broadcast();
}

void USESaveGameItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OnClicked.AddDynamic(this, &USESaveGameItemWidget::SaveLevel);
}

void USESaveGameItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (MenuWidget != nullptr)
	{
		MenuWidget->SetSelected(this);
	}

	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void USESaveGameItemWidget::SaveLevel()
{
	if (GetWorld() == nullptr)
	{
		return;
	}

	ASEGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASEGameModeBase>();
	if (GameMode == nullptr)
	{
		return;
	}
	
	USESaveGame* LoadGameInstance = GameMode->SaveGameToSlot(SlotName);

	SaveDate->SetText(FText::FromString(LoadGameInstance->SaveTime.ToString(TEXT("%d.%m.%y-%H:%M"))));
}
