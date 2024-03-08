// Silent Evil Game, All Rights Reserved.

#include "UI/SEGameHUD.h"

#include "Blueprint/UserWidget.h"
#include "SEGameModeBase.h"

#include "UI/SEInventoryContainerWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogSEGameHUD, All, All);

void ASEGameHUD::OpenTargetInventory(bool ShowItems)
{
	if (InventoryWidget == nullptr)
	{
		return;
	}

	InventoryWidget->Open(true, ShowItems);
}

void ASEGameHUD::OpenInventory()
{
	if (InventoryWidget == nullptr)
	{
		return;
	}

	InventoryWidget->Open();
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

void ASEGameHUD::BeginPlay()
{
	Super::BeginPlay();

	InventoryWidget = CreateWidget<USEInventoryContainerWidget>(GetWorld(), InventoryWidgetClass);
	if (InventoryWidget != nullptr)
	{
		InventoryWidget->AddToViewport();
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	PauseMenuWidget = CreateWidget<UUserWidget>(GetWorld(), PauseMenuWidgetClass);
	if (PauseMenuWidget != nullptr)
	{
		PauseMenuWidget->AddToViewport();
		PauseMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
	if (PlayerHUDWidget != nullptr)
	{
		PlayerHUDWidget->AddToViewport();
	}
}
