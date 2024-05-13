// Silent Evil Game, All Rights Reserved.

#include "UI/SEGameHUD.h"

#include "Blueprint/UserWidget.h"
#include "SEGameModeBase.h"

#include "UI/SESubmenuWidget.h"
#include "UI/InventoryWidgets/SEInventoryContainerWidget.h"
#include "UI/PauseMenuWidgets/SEPauseMenuWidget.h"
#include "UI/SEPlayerHUDWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogSEGameHUD, All, All);

void ASEGameHUD::OpenTargetInventory(bool ShowItems)
{
	if (InventoryWidget == nullptr)
	{
		return;
	}

	InventoryWidget->OpenItems(true, ShowItems);
}

void ASEGameHUD::OpenInventory()
{
	if (InventoryWidget == nullptr)
	{
		return;
	}

	InventoryWidget->OpenItems();
}

void ASEGameHUD::OpenRecords()
{
	if (InventoryWidget == nullptr)
	{
		return;
	}

	InventoryWidget->OpenRecords();
}

void ASEGameHUD::OpenMap()
{
	if (InventoryWidget == nullptr)
	{
		return;
	}

	InventoryWidget->OpenMap();
}

void ASEGameHUD::OpenTargetWiget(USESubmenuWidget* WidgetToFocus)
{
	if (!WidgetToFocus->IsInViewport())
	{
		WidgetToFocus->AddToViewport();
	}

	PauseMenuWidget->CurrentTargetWidget = WidgetToFocus;
	WidgetToFocus->Open();
}

void ASEGameHUD::OpenMenu()
{
	if (PauseMenuWidget == nullptr)
	{
		return;
	}

	PauseMenuWidget->SetVisibility(ESlateVisibility::Visible);
}

void ASEGameHUD::Close()
{
	/*if (CurrentTargetWidget)
	{
		CurrentTargetWidget->SetVisibility(ESlateVisibility::Collapsed);
		CurrentTargetWidget->RemoveFromParent();
		CurrentTargetWidget = nullptr;

		return;
	}*/

	if (PauseMenuWidget != nullptr && PauseMenuWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		PauseMenuWidget->SetVisibility(ESlateVisibility::Collapsed);

		return;
	}

	if (InventoryWidget != nullptr && InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);

		return;
	}
}

void ASEGameHUD::ShowHelpText(FText InText, float Time)
{
	if (PlayerHUDWidget != nullptr)
	{
		PlayerHUDWidget->SetHelpText(InText, Time);
	}
}

USEInventoryContainerWidget* ASEGameHUD::GetInventoryWidget()
{
	return InventoryWidget;
}

void ASEGameHUD::BeginPlay()
{
	Super::BeginPlay();

	InventoryWidget = CreateWidget<USEInventoryContainerWidget>(GetWorld(), InventoryWidgetClass);
	if (InventoryWidget != nullptr)
	{
		InventoryWidget->AddToViewport();
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	PauseMenuWidget = CreateWidget<USEPauseMenuWidget>(GetWorld(), PauseMenuWidgetClass);
	if (PauseMenuWidget != nullptr)
	{
		PauseMenuWidget->AddToViewport();
		PauseMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	PlayerHUDWidget = CreateWidget<USEPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
	if (PlayerHUDWidget != nullptr)
	{
		PlayerHUDWidget->AddToViewport();
	}
}
