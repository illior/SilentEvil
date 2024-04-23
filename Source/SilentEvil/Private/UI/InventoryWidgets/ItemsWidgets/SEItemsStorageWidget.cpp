// Silent Evil Game, All Rights Reserved.

#include "UI/InventoryWidgets/ItemsWidgets/SEItemsStorageWidget.h"

#include "UI/InventoryWidgets/ItemsWidgets/SEStorageButtonWidget.h"
#include "Components/ScrollBox.h"
#include "InventorySystem/SEItemData.h"
#include "Components/SEInventoryComponent.h"
#include "Player/SEPlayerState.h"

void USEItemsStorageWidget::Hide()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void USEItemsStorageWidget::Show()
{
	SetVisibility(ESlateVisibility::Visible);

	CurrentIndex = 0;
}

void USEItemsStorageWidget::AddItem(USEItemData* ItemData)
{
	APlayerController* CharacterController = GetOwningPlayer();
	if (CharacterController == nullptr)
	{
		return;
	}

	ASEPlayerState* PlayerState = Cast<ASEPlayerState>(CharacterController->PlayerState);
	if (PlayerState == nullptr)
	{
		return;
	}

	bool ShouldCreateWidget = PlayerState->AddItemToStorage(ItemData);
	if (ShouldCreateWidget)
	{
		USEStorageButtonWidget* StorageItem = CreateWidget<USEStorageButtonWidget>(GetWorld(), ButtonClass);
		if (StorageItem == nullptr)
		{
			return;
		}

		StorageItem->SetItemData(ItemData);
		StorageItem->SetState(ESEButtonState::Normal);

		Scroll->AddChild(StorageItem);
		StorageItems.Add(StorageItem);
	}
}

void USEItemsStorageWidget::RemoveCurrentItem()
{
	APlayerController* CharacterController = GetOwningPlayer();
	if (CharacterController == nullptr)
	{
		return;
	}

	ASEPlayerState* PlayerState = Cast<ASEPlayerState>(CharacterController->PlayerState);
	if (PlayerState == nullptr)
	{
		return;
	}

	USEStorageButtonWidget* WidgetToDestroy = StorageItems[CurrentIndex];

	PlayerState->RemoveItemFromStorage(WidgetToDestroy->GetItemData());
	StorageItems.Remove(WidgetToDestroy);

	WidgetToDestroy->RemoveFromParent();
	WidgetToDestroy->MarkAsGarbage();

	CurrentIndex = FMath::Clamp(CurrentIndex - 1, 0, StorageItems.Num() - 1);
	if (StorageItems.IsValidIndex(CurrentIndex))
	{
		StorageItems[CurrentIndex]->SetState(ESEButtonState::Hovered);
	}
}

bool USEItemsStorageWidget::SetFocus()
{
	if (StorageItems.Num() == 0)
	{
		return false;
	}

	CurrentIndex = 0;
	StorageItems[CurrentIndex]->SetState(ESEButtonState::Hovered);

	return true;
}

void USEItemsStorageWidget::ClearFocus()
{
	if (StorageItems.IsValidIndex(CurrentIndex))
	{
		StorageItems[CurrentIndex]->SetState(ESEButtonState::Normal);
	}
}

void USEItemsStorageWidget::MoveUp()
{
	if (StorageItems.Num() == 0)
	{
		return;
	}

	StorageItems[CurrentIndex]->SetState(ESEButtonState::Normal);

	CurrentIndex = FMath::Clamp(CurrentIndex + 1, 0, StorageItems.Num() - 1);
	StorageItems[CurrentIndex]->SetState(ESEButtonState::Hovered);

	Scroll->ScrollWidgetIntoView(StorageItems[CurrentIndex]);
}

void USEItemsStorageWidget::MoveDown()
{
	if (StorageItems.Num() == 0)
	{
		return;
	}

	StorageItems[CurrentIndex]->SetState(ESEButtonState::Normal);

	CurrentIndex = FMath::Clamp(CurrentIndex - 1, 0, StorageItems.Num() - 1);
	StorageItems[CurrentIndex]->SetState(ESEButtonState::Hovered);

	Scroll->ScrollWidgetIntoView(StorageItems[CurrentIndex]);
}

void USEItemsStorageWidget::Apply()
{
	if (!StorageItems.IsValidIndex(CurrentIndex))
	{
		UE_LOG(LogTemp, Display, TEXT("index nullptr"));
		return;
	}

	USEItemData* ItemData = StorageItems[CurrentIndex]->GetItemData();
	if (ItemData == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("ItemData nullptr"));
		return;
	}
	
	const APawn* Character = GetOwningPlayerPawn();
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Character nullptr"));
		return;
	}

	USEInventoryComponent* CharacterInventory = Character->GetComponentByClass<USEInventoryComponent>();
	if (CharacterInventory == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("CharacterInventory nullptr"));
		return;
	}

	APlayerController* CharacterController = GetOwningPlayer();
	if (CharacterController == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("CharacterController nullptr"));
		return;
	}

	ASEPlayerState* PlayerState = Cast<ASEPlayerState>(CharacterController->PlayerState);
	if (PlayerState == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("PlayerState nullptr"));
		return;
	}

	if (CharacterInventory->TryAddItem(ItemData))
	{
		RemoveCurrentItem();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("false"));
	}
}

bool USEItemsStorageWidget::IsEmpty()
{
	return StorageItems.Num() == 0;
}

void USEItemsStorageWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	APlayerController* CharacterController = GetOwningPlayer();
	if (CharacterController == nullptr)
	{
		return;
	}

	ASEPlayerState* PlayerState = Cast<ASEPlayerState>(CharacterController->PlayerState);
	if (PlayerState == nullptr)
	{
		return;
	}

	Scroll->ClearChildren();

	TArray<USEItemData*> Items = PlayerState->GetItemsInStorage();
	for (USEItemData* Item : Items)
	{
		USEStorageButtonWidget* StorageItem = CreateWidget<USEStorageButtonWidget>(GetWorld(), ButtonClass);
		if (StorageItem == nullptr)
		{
			return;
		}

		StorageItem->SetItemData(Item);
		StorageItem->SetState(ESEButtonState::Normal);

		Scroll->AddChild(StorageItem);
		StorageItems.Add(StorageItem);
	}
}
