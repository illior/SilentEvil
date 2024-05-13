// Silent Evil Game, All Rights Reserved.

#include "UI/InventoryWidgets/ItemsWidgets/SEInventoryWidget.h"

#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "UI/InventoryWidgets/ItemsWidgets/SEInventorySlotWidget.h"
#include "UI/InventoryWidgets/ItemsWidgets/SEInventoryItemWidget.h"
#include "UI/InventoryWidgets/ItemsWidgets/SEInventoryCursorWidget.h"
#include "UI/InventoryWidgets/ItemsWidgets/SEDropDownMenuWidget.h"
#include "UI/InventoryWidgets/ItemsWidgets/SEInspectWidget.h"
#include "UI/InventoryWidgets/ItemsWidgets/SEItemsStorageWidget.h"

#include "Player/SEPlayerController.h"
#include "Components/SEInventoryComponent.h"
#include "Components/SEHealthComponent.h"
#include "InventorySystem/SEItemData.h"
#include "InventorySystem/SEWeaponData.h"
#include "InventorySystem/Interfaces/SEUsableItemInterface.h"
#include "InventorySystem/Actors/SEItemVisual.h"
#include "InteractableObjects/SEInteractableTarget.h"
#include "InteractableObjects/SEStorageBox.h"

DEFINE_LOG_CATEGORY_STATIC(LogUInventory, All, All);

bool USEInventoryWidget::Cancel()
{
	switch (InventoryState)
	{
	case ESEInventoryState::Selected:
		return true;
	case ESEInventoryState::DropDownMenu:
		CloseDropDownMenu();
		return false;
	case ESEInventoryState::ItemMoving:
		CancelMoveItem();
		return false;
	case ESEInventoryState::ListenBind:
		StopFastAccess();
		return false;
	case ESEInventoryState::ItemInspect:
		if (CurrentItemWidget->GetItemData()->GetItemVisual()->Cancel())
		{
			StopInspectItem();
		}
		return false;
	case ESEInventoryState::Storage:
		MoveCuresorFromStorage();
		return false;
	default:
		break;
	}

	return true;
}

void USEInventoryWidget::MoveItem()
{
	switch (InventoryState)
	{
	case ESEInventoryState::Selected:
		StartMoveItem();
		break;
	case ESEInventoryState::DropDownMenu:
		break;
	case ESEInventoryState::ItemMoving:
		StopMoveItem();
		break;
	case ESEInventoryState::ListenBind:
		StopFastAccess();
		break;
	case ESEInventoryState::ItemInspect:
		break;
	case ESEInventoryState::Storage:
		break;
	default:
		break;
	}
}

void USEInventoryWidget::MoveVertical(float Value)
{
	switch (InventoryState)
	{
	case ESEInventoryState::Selected:
		MoveItemTo(FIntPoint(0, (int32)Value));
		break;
	case ESEInventoryState::DropDownMenu:
		Value > 0 ? DropDownMenu->MoveSelectedUp() : DropDownMenu->MoveSelectedDown();
		break;
	case ESEInventoryState::ItemMoving:
		MoveItemTo(FIntPoint(0, (int32)Value));
		break;
	case ESEInventoryState::ListenBind:
		StopFastAccess();
		break;
	case ESEInventoryState::ItemInspect:
		CurrentItemWidget->GetItemData()->GetItemVisual()->VerticalInput(Value);
		break;
	case ESEInventoryState::Storage:
		StorageMoveCursor((int32)-Value);
		break;
	default:
		break;
	}
}

void USEInventoryWidget::MoveHorizontal(float Value)
{
	switch (InventoryState)
	{
	case ESEInventoryState::Selected:
		MoveItemTo(FIntPoint((int32)Value, 0));
		break;
	case ESEInventoryState::DropDownMenu:
		CloseDropDownMenu();
		break;
	case ESEInventoryState::ItemMoving:
		MoveItemTo(FIntPoint((int32)Value, 0));
		break;
	case ESEInventoryState::ListenBind:
		StopFastAccess();
		break;
	case ESEInventoryState::ItemInspect:
		CurrentItemWidget->GetItemData()->GetItemVisual()->HorizontalInput(Value);
		break;
	case ESEInventoryState::Storage:
		break;
	default:
		break;
	}
}

void USEInventoryWidget::FastAccessItem(float Value)
{
	if (InventoryState == ESEInventoryState::ListenBind)
	{
		SetFastAccess((int32)Value - 1);
	}
}

void USEInventoryWidget::Apply()
{
	switch (InventoryState)
	{
	case ESEInventoryState::Selected:
		OpenDropDownMenu();
		break;
	case ESEInventoryState::DropDownMenu:
		DropDownMenu->ActivateSelected();
		break;
	case ESEInventoryState::ItemMoving:
		StopMoveItem();
		break;
	case ESEInventoryState::ListenBind:
		StopFastAccess();
		break;
	case ESEInventoryState::ItemInspect:
		CurrentItemWidget->GetItemData()->GetItemVisual()->Apply();
		break;
	case ESEInventoryState::Storage:
		ItemsStorage->Apply();

		if (ItemsStorage->IsEmpty())
		{
			MoveCuresorFromStorage();
		}

		break;
	default:
		break;
	}
}

void USEInventoryWidget::UpdateInventoryData(bool OpenForInteraction)
{
	CurrentItemWidget = nullptr;

	InspectPanel->SetVisibility(ESlateVisibility::Collapsed);
	ItemsStorage->Hide();

	if (InventoryItems.Num() == 0)
	{
		UpdateSlots();
		UpdateItems();
	}

	if (OpenForInteraction)
	{
		CurrentState = ESEInventoryOpenState::Interaction;

		ASEPlayerController* CharacterController = Cast<ASEPlayerController>(GetOwningPlayer());
		if (CharacterController != nullptr && CharacterController->GetViewTarget() != nullptr && CharacterController->GetViewTarget()->IsA<ASEStorageBox>())
		{
			CurrentState = ESEInventoryOpenState::Storage;
			ItemsStorage->Show();
		}

		HideFastAccessItems();
		HideHealth();
	}
	else
	{
		CurrentState = ESEInventoryOpenState::Base;

		UpdateFastAccessItems();
		UpdateHealth();
	}
	

	SetCurrentSelected();
	InventoryCursor->SetViewState(EViewState::Normal);

	CloseDropDownMenu();

	InventoryState = ESEInventoryState::Selected;
}

void USEInventoryWidget::RemoveItem(USEInventoryItemWidget* ItemWidget)
{
	if (InventoryItems.Contains(ItemWidget))
	{
		if (CurrentItemWidget == ItemWidget)
		{
			CurrentItemWidget = nullptr;
		}

		USEItemData* DroppedItem = ItemWidget->GetItemData();
		if (DroppedItem == nullptr)
		{
			return;
		}
		
		InventorySlots[DroppedItem->GetPosition()]->SetState(ESlotState::Void);
		if (DroppedItem->GetItemSize() == 2)
		{
			InventorySlots[FIntPoint(DroppedItem->GetPosition().X + 1, DroppedItem->GetPosition().Y)]->SetState(ESlotState::Void);
		}

		InventoryItems.Remove(ItemWidget);

		ItemWidget->RemoveFromParent();
		ItemWidget->MarkAsGarbage();
	}
}

void USEInventoryWidget::UpdatePosition(USEInventoryItemWidget* ItemWidget)
{
	if (InventoryItems.Contains(ItemWidget))
	{
		USEItemData* ItemData = ItemWidget->GetItemData();
		FIntPoint Position = ItemData->GetPosition();

		InventorySlots[Position]->SetState(ESlotState::Item);

		if (ItemData->GetItemSize() == 2)
		{
			Position.X += 1;
			InventorySlots[Position]->SetState(ESlotState::Item);
		}
	}
}

void USEInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CreateSlots();
	SetupFastAccessItems();

	InventoryCursor->Margin = SlotsGridPanel->GetSlotPadding();
	InventoryCursor->SetPosition(FIntPoint(0, 0));

	DropDownMenu->GetExploreButtonSignature().AddDynamic(this, &USEInventoryWidget::ExploreItem);
	DropDownMenu->GetUseButtonSignature().AddDynamic(this, &USEInventoryWidget::UseItem);
	DropDownMenu->GetEquipButtonSignature().AddDynamic(this, &USEInventoryWidget::EquipWeaponItem);
	DropDownMenu->GetFastAccessButtonSignature().AddDynamic(this, &USEInventoryWidget::FastAccessWeaponItem);
	DropDownMenu->GetStoreButtonSignature().AddDynamic(this, &USEInventoryWidget::StoreItem);
	DropDownMenu->GetCombineButtonSignature().AddDynamic(this, &USEInventoryWidget::StartMoveItem);
	DropDownMenu->GetDropButtonSignature().AddDynamic(this, &USEInventoryWidget::DropItem);

	USEInventoryComponent* CharacterInventory = GetCharacterInventory();
	if (CharacterInventory != nullptr)
	{
		CharacterInventory->OnItemAdd.AddDynamic(this, &USEInventoryWidget::OnItemAdd);
	}
}

USEInventoryComponent* USEInventoryWidget::GetCharacterInventory()
{
	const APawn* Character = GetOwningPlayerPawn();
	if (Character == nullptr)
	{
		return nullptr;
	}

	USEInventoryComponent* CharacterInventory = Character->GetComponentByClass<USEInventoryComponent>();

	return CharacterInventory;
}

void USEInventoryWidget::MoveItemTo(const FIntPoint& Offset)
{
	if (InventoryCursor == nullptr)
	{
		return;
	}

	FIntPoint NewPosition = InventoryCursor->GetPosition();
	NewPosition.X += Offset.X;
	NewPosition.Y -= Offset.Y;

	if (CurrentState == ESEInventoryOpenState::Storage && NewPosition.X == -1 && !ItemsStorage->IsEmpty())
	{
		MoveCursorToStorage();
		return;
	}

	if (!InventorySlots.Contains(NewPosition) || InventorySlots[NewPosition]->GetState() == ESlotState::Block)
	{
		return;
	}

	switch (InventoryState)
	{
	case ESEInventoryState::Selected:
		SetCurrentUnSelected();
		InventoryCursor->SetPosition(NewPosition);
		SetCurrentSelected();
		break;
	case ESEInventoryState::ItemMoving:
		InventoryCursor->SetPosition(NewPosition);
		break;
	}
}

void USEInventoryWidget::SetCurrentSelected()
{
	if (InventorySlots[InventoryCursor->GetPosition()]->GetState() == ESlotState::Item)
	{
		USEInventoryItemWidget** PointCurrentItemWidget = InventoryItems.FindByPredicate([&](USEInventoryItemWidget* ItemWidget) {
			return ItemWidget->GetItemData()->ContainsPoint(InventoryCursor->GetPosition());
		});


		CurrentItemWidget = PointCurrentItemWidget != nullptr ? *PointCurrentItemWidget : nullptr;

		if (CurrentItemWidget != nullptr)
		{
			CurrentItemWidget->SetViewState(EViewState::Selected);

			if (ItemName != nullptr && ItemDescription != nullptr)
			{
				ItemName->SetText(CurrentItemWidget->GetItemData()->GetItemName());
				ItemDescription->SetText(CurrentItemWidget->GetItemData()->GetItemDescription());
			}
		}
	}
	else
	{
		CurrentItemWidget = nullptr;
		InventorySlots[InventoryCursor->GetPosition()]->SetViewState(EViewState::Selected);

		if (ItemName != nullptr && ItemDescription != nullptr)
		{
			ItemName->SetText(FText());
			ItemDescription->SetText(FText());
		}
	}
}

void USEInventoryWidget::SetCurrentUnSelected()
{
	if (CurrentItemWidget != nullptr)
	{
		CurrentItemWidget->SetViewState(EViewState::Normal);
		CurrentItemWidget = nullptr;

		if (ItemName != nullptr)
		{
			ItemName->SetText(FText::FromString(""));
		}

		return;
	}

	InventorySlots[InventoryCursor->GetPosition()]->SetViewState(EViewState::Normal);
}

void USEInventoryWidget::SetFastAccess(int32 Index)
{
	USEInventoryComponent* CharacterInventory = GetCharacterInventory();
	if (CharacterInventory == nullptr)
	{
		return;
	}

	if (CurrentItemWidget == nullptr)
	{
		return;
	}

	if (FastAccessItems.IsValidIndex(Index))
	{
		USEInventoryItemWidget** PointWeaponItemWidget = FastAccessItems.FindByPredicate([&](USEInventoryItemWidget* ItemWidget) {
			return ItemWidget->GetItemData() == CurrentItemWidget->GetItemData();
		});

		if (PointWeaponItemWidget != nullptr && *PointWeaponItemWidget != nullptr)
		{
			(*PointWeaponItemWidget)->SetItemData(nullptr);

			if (FastAccessItems[Index]->GetItemData() != nullptr)
			{
				(*PointWeaponItemWidget)->SetItemData(FastAccessItems[Index]->GetItemData());
			}
		}

		FastAccessItems[Index]->SetItemData(nullptr);
		FastAccessItems[Index]->SetItemData(CurrentItemWidget->GetItemData());

		CharacterInventory->SetFastAccessWeapon(CurrentItemWidget->GetItemData(), Index);
	}

	InventoryState = ESEInventoryState::Selected;
	UE_LOG(LogUInventory, Display, TEXT("FastAccess %i"), Index);
}

void USEInventoryWidget::StopFastAccess()
{
	InventoryState = ESEInventoryState::Selected;
	UE_LOG(LogUInventory, Display, TEXT("Stop FastAccess"));
}

void USEInventoryWidget::StopMoveItem()
{
	if (InventoryCursor == nullptr || CurrentItemWidget == nullptr)
	{
		return;
	}

	if (InventoryCursor->GetPosition() != CurrentItemWidget->GetItemData()->GetPosition())
	{
		USEInventoryComponent* CharacterInventory = GetCharacterInventory();
		if (CharacterInventory == nullptr)
		{
			return;
		}

		CurrentItemWidget->SetViewState(EViewState::Normal);
		const bool Result = CharacterInventory->MoveItemTo(CurrentItemWidget->GetItemData(), InventoryCursor->GetPosition().X, InventoryCursor->GetPosition().Y);
		
		if (Result)
		{
			InventoryCursor->SetViewState(EViewState::Normal);
			InventoryState = ESEInventoryState::Selected;

			SetCurrentSelected();
		}
		else
		{
			CurrentItemWidget->SetViewState(EViewState::Active);
		}
	}
	else
	{
		InventoryCursor->SetViewState(EViewState::Normal);
		InventoryState = ESEInventoryState::Selected;

		CurrentItemWidget->SetViewState(EViewState::Selected);

		USEItemData* ItemData = CurrentItemWidget->GetItemData();
		FIntPoint Position = ItemData->GetPosition();

		InventorySlots[Position]->SetState(ESlotState::Item);

		if (ItemData->GetItemSize() == 2)
		{
			Position.X += 1;
			InventorySlots[Position]->SetState(ESlotState::Item);
		}
	}
}

void USEInventoryWidget::CancelMoveItem()
{
	if (InventoryCursor == nullptr || CurrentItemWidget == nullptr)
	{
		return;
	}

	CurrentItemWidget->SetViewState(EViewState::Normal);
	InventoryCursor->SetViewState(EViewState::Normal);
	InventoryState = ESEInventoryState::Selected;

	USEItemData* ItemData = CurrentItemWidget->GetItemData();
	FIntPoint Position = ItemData->GetPosition();

	InventorySlots[Position]->SetState(ESlotState::Item);

	if (ItemData->GetItemSize() == 2)
	{
		Position.X += 1;
		InventorySlots[Position]->SetState(ESlotState::Item);
	}

	SetCurrentSelected();
}

void USEInventoryWidget::OpenDropDownMenu()
{
	if (CurrentItemWidget == nullptr || DropDownMenu == nullptr)
	{
		return;
	}

	UCanvasPanelSlot* MenuSlot = Cast<UCanvasPanelSlot>(DropDownMenu->Slot);
	UCanvasPanelSlot* CursorSlot = Cast<UCanvasPanelSlot>(InventoryCursor->Slot);

	if (MenuSlot == nullptr || CursorSlot == nullptr)
	{
		return;
	}

	if (CurrentItemWidget->GetItemData()->GetItemSize() == 2 && CurrentItemWidget->GetItemData()->GetPosition() == InventoryCursor->GetPosition())
	{
		MenuSlot->SetPosition(CursorSlot->GetPosition()
			+ FVector2D(CursorSlot->GetSize().X * 2 + (SlotsGridPanel->GetSlotPadding().Right + SlotsGridPanel->GetSlotPadding().Left), 0.0f));
	}
	else
	{
		MenuSlot->SetPosition(CursorSlot->GetPosition() + FVector2D(CursorSlot->GetSize().X, 0.0f));
	}

	CurrentItemWidget->SetViewState(EViewState::Active);
	DropDownMenu->Open(CurrentItemWidget->GetItemData(), CurrentState);
	InventoryState = ESEInventoryState::DropDownMenu;
}

void USEInventoryWidget::CloseDropDownMenu()
{
	if (DropDownMenu == nullptr)
	{
		return;
	}

	if (CurrentItemWidget != nullptr)
	{
		CurrentItemWidget->SetViewState(EViewState::Selected);
	}
	
	DropDownMenu->Close();
	InventoryState = ESEInventoryState::Selected;
}

void USEInventoryWidget::StartInspectItem()
{
	InventoryState = ESEInventoryState::ItemInspect;

	ASEItemVisual* ItemActor = CurrentItemWidget->GetItemData()->GetItemVisual();
	ItemActor->SetActorHiddenInGame(false);

	InspectPanel->SetVisibility(ESlateVisibility::Visible);
	UE_LOG(LogUInventory, Display, TEXT("StartInspectItem"));
}

void USEInventoryWidget::StopInspectItem()
{
	InventoryState = ESEInventoryState::Selected;

	ASEItemVisual* ItemActor = CurrentItemWidget->GetItemData()->GetItemVisual();
	ItemActor->SetActorHiddenInGame(true);

	InspectPanel->SetVisibility(ESlateVisibility::Collapsed);
	UE_LOG(LogUInventory, Display, TEXT("StopInspectItem"));
}

void USEInventoryWidget::MoveCursorToStorage()
{
	if (!ItemsStorage->SetFocus())
	{
		return;
	}

	InventoryCursor->SetVisibility(ESlateVisibility::Hidden);
	SetCurrentUnSelected();
	
	InventoryState = ESEInventoryState::Storage;
	UE_LOG(LogUInventory, Display, TEXT("ToStorage"));
}

void USEInventoryWidget::MoveCuresorFromStorage()
{
	ItemsStorage->ClearFocus();

	InventoryCursor->SetVisibility(ESlateVisibility::Visible);
	SetCurrentSelected();

	InventoryState = ESEInventoryState::Selected;
	UE_LOG(LogUInventory, Display, TEXT("FromStorage"));
}

void USEInventoryWidget::StorageMoveCursor(int32 Value)
{
	Value < 0 ? ItemsStorage->MoveDown() : ItemsStorage->MoveUp();
}

void USEInventoryWidget::StartMoveItem()
{
	if (InventoryCursor == nullptr || CurrentItemWidget == nullptr)
	{
		return;
	}

	if (InventoryState == ESEInventoryState::DropDownMenu)
	{
		CloseDropDownMenu();
	}

	InventoryState = ESEInventoryState::ItemMoving;
	InventoryCursor->SetViewState(EViewState::Active);
	CurrentItemWidget->SetViewState(EViewState::Active);

	USEItemData* ItemData = CurrentItemWidget->GetItemData();
	FIntPoint Position = ItemData->GetPosition();

	InventorySlots[Position]->SetState(ESlotState::Void);

	if (ItemData->GetItemSize() == 2)
	{
		Position.X += 1;
		InventorySlots[Position]->SetState(ESlotState::Void);
	}
}

void USEInventoryWidget::ExploreItem()
{
	if (CurrentItemWidget == nullptr || DropDownMenu == nullptr)
	{
		return;
	}

	CloseDropDownMenu();

	if (CurrentItemWidget->GetItemData()->GetItemVisual() == nullptr)
	{
		UE_LOG(LogUInventory, Display, TEXT("Explore nullptr"));
	}
	else
	{
		StartInspectItem();
	}
}

void USEInventoryWidget::UseItem()
{
	if (CurrentItemWidget == nullptr)
	{
		return;
	}

	switch (CurrentState)
	{
	case ESEInventoryOpenState::Base:
		UseSelfItem();
		break;
	case ESEInventoryOpenState::Interaction:
		UseItemOnObject();
		break;
	case ESEInventoryOpenState::Storage:
		break;
	default:
		break;
	}

	CloseDropDownMenu();
	SetCurrentSelected();

	UE_LOG(LogUInventory, Display, TEXT("Use"));
}

void USEInventoryWidget::EquipWeaponItem()
{
	if (CurrentItemWidget == nullptr || DropDownMenu == nullptr)
	{
		return;
	}

	CloseDropDownMenu();

	USEInventoryComponent* CharacterInventory = GetCharacterInventory();
	if (CharacterInventory == nullptr)
	{
		return;
	}

	if (CurrentItemWidget->GetItemData()->IsWeapon())
	{
		CharacterInventory->SetEquipWeapon(CurrentItemWidget->GetItemData());
	}
}

void USEInventoryWidget::FastAccessWeaponItem()
{
	if (CurrentItemWidget == nullptr || DropDownMenu == nullptr)
	{
		return;
	}

	CloseDropDownMenu();
	InventoryState = ESEInventoryState::ListenBind;

	UE_LOG(LogUInventory, Display, TEXT("FastAccess Start"));
}

void USEInventoryWidget::DropItem()
{
	if (CurrentItemWidget == nullptr)
	{
		return;
	}

	USEInventoryComponent* CharacterInventory = GetCharacterInventory();
	if (CharacterInventory == nullptr)
	{
		return;
	}

	CloseDropDownMenu();

	bool Result = CharacterInventory->DropItem(CurrentItemWidget->GetItemData());
	if (Result)
	{
		CurrentItemWidget = nullptr;
		SetCurrentSelected();
	}

	UE_LOG(LogUInventory, Display, TEXT("Drop"));
}

void USEInventoryWidget::StoreItem()
{
	if (CurrentItemWidget == nullptr)
	{
		return;
	}
	USEItemData* CurrentItem = CurrentItemWidget->GetItemData();

	USEInventoryComponent* CharacterInventory = GetCharacterInventory();
	if (CharacterInventory == nullptr)
	{
		return;
	}

	CharacterInventory->DropItem(CurrentItem, false);
	ItemsStorage->AddItem(CurrentItem);

	CurrentItemWidget = nullptr;
	SetCurrentSelected();
	CloseDropDownMenu();

	UE_LOG(LogUInventory, Display, TEXT("Store"));
}

void USEInventoryWidget::OnItemAdd(USEItemData* NewItemData)
{
	if (ItemsPanel == nullptr || SlotsGridPanel == nullptr)
	{
		return;
	}

	if (InventoryItems.Num() == 0)
	{
		UpdateSlots();
		UpdateItems();
	}

	USEInventoryItemWidget* InventorySlotWidget = CreateWidget<USEInventoryItemWidget>(GetWorld(), InventoryItemWidgetClass);
	if (InventorySlotWidget == nullptr)
	{
		return;
	}

	UGridSlot* GridSlot = ItemsPanel->AddChildToGrid(InventorySlotWidget);
	GridSlot->SetPadding(SlotsGridPanel->GetSlotPadding());
	
	InventorySlotWidget->SetItemData(NewItemData);
	InventorySlotWidget->ParentWidget = this;

	InventoryItems.Add(InventorySlotWidget);

	UE_LOG(LogUInventory, Display, TEXT("Event item add"));
	UpdateSlots();
}

void USEInventoryWidget::UseSelfItem()
{
	USEInventoryComponent* CharacterInventory = GetCharacterInventory();
	if (CharacterInventory == nullptr)
	{
		return;
	}

	CharacterInventory->UseItem(CurrentItemWidget->GetItemData());

	UpdateHealth();
}

void USEInventoryWidget::UseItemOnObject()
{
	ASEPlayerController* CharacterController = Cast<ASEPlayerController>(GetOwningPlayer());
	if (CharacterController == nullptr)
	{
		return;
	}

	ASEInteractableTarget* TargetObject = Cast<ASEInteractableTarget>(CharacterController->GetViewTarget());
	if (TargetObject == nullptr)
	{
		return;
	}

	if (TargetObject->TryUseItem(CurrentItemWidget->GetItemData()->GetItem()) && TargetObject->GetRemoveItem())
	{
		DropItem();
	}
}

void USEInventoryWidget::CreateSlots()
{
	const USEInventoryComponent* CharacterInventory = GetCharacterInventory();
	if (CharacterInventory == nullptr)
	{
		return;
	}

	if (SlotsGridPanel == nullptr)
	{
		return;
	}

	SlotsGridPanel->ClearChildren();
	InventorySlots.Empty();
	const FIntPoint InventorySize = CharacterInventory->GetMaxSize();

	for (int32 Y = 0; Y < InventorySize.Y; Y++)
	{
		for (int32 X = 0; X < InventorySize.X; X++)
		{
			USEInventorySlotWidget* InventorySlotWidget = CreateWidget<USEInventorySlotWidget>(GetWorld(), InventorySlotWidgetClass);
			if (InventorySlotWidget == nullptr)
			{
				continue;
			}

			UUniformGridSlot* GridSlot = SlotsGridPanel->AddChildToUniformGrid(InventorySlotWidget, Y, X);
			GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

			InventorySlots.Add(FIntPoint(X, Y), InventorySlotWidget);
		}
	}
}

void USEInventoryWidget::UpdateSlots()
{
	const USEInventoryComponent* CharacterInventory = GetCharacterInventory();
	if (CharacterInventory == nullptr)
	{
		return;
	}

	if (SlotsGridPanel == nullptr)
	{
		return;
	}

	const FIntPoint InventorySize = CharacterInventory->GetMaxSize();
	for (int32 Y = 0; Y < InventorySize.Y; Y++)
	{
		for (int32 X = 0; X < InventorySize.X; X++)
		{
			FIntPoint Position(X, Y);
			if (InventorySlots.Contains(Position))
			{
				InventorySlots[Position]->SetState(CharacterInventory->GetSlotState(Position));
				InventorySlots[Position]->SetViewState(EViewState::Normal);
			}
		}
	}
}

void USEInventoryWidget::HideFastAccessItems()
{
	FirstAccessItemsPanel->SetVisibility(ESlateVisibility::Hidden);
}

void USEInventoryWidget::SetupFastAccessItems()
{
	TArray<UWidget*> FirstAccesSlots = FirstAccessItemsPanel->GetAllChildren();

	for (int32 i = 0; i < FirstAccesSlots.Num(); i++)
	{
		USEInventoryItemWidget* FastAccessItemWidget = Cast<USEInventoryItemWidget>(FirstAccesSlots[i]);

		if (FastAccessItemWidget == nullptr)
		{
			continue;
		}

		FastAccessItemWidget->SetItemData(nullptr);
		FastAccessItems.Add(FastAccessItemWidget);
	}
}

void USEInventoryWidget::UpdateFastAccessItems()
{
	const USEInventoryComponent* CharacterInventory = GetCharacterInventory();
	if (CharacterInventory == nullptr)
	{
		return;
	}

	USEWeaponData* EquipWeapon = CharacterInventory->GetEquipWeapon();

	FirstAccessItemsPanel->SetVisibility(ESlateVisibility::Visible);
	for (int32 i = 0; i < FastAccessItems.Num(); i++)
	{
		USEWeaponData* WeaponData = CharacterInventory->GetFastAccessWeapon(i);

		if (FastAccessItems[i]->GetItemData() != nullptr)
		{
			FastAccessItems[i]->SetItemData(nullptr);
		}
		FastAccessItems[i]->SetItemData(WeaponData);
		FastAccessItems[i]->SetVisibility(ESlateVisibility::Visible);
	}
}

void USEInventoryWidget::UpdateItems()
{
	const USEInventoryComponent* CharacterInventory = GetCharacterInventory();
	if (CharacterInventory == nullptr)
	{
		return;
	}

	if (ItemsPanel == nullptr || SlotsGridPanel == nullptr)
	{
		return;
	}

	ItemsPanel->ClearChildren();
	InventoryItems.Empty();
	for (TIndexedContainerIterator It = CharacterInventory->GetItemsIterator(); It; ++It)
	{
		if (*It == nullptr)
		{
			continue;
		}

		USEInventoryItemWidget* InventorySlotWidget = CreateWidget<USEInventoryItemWidget>(GetWorld(), InventoryItemWidgetClass);
		if (InventorySlotWidget == nullptr)
		{
			continue;
		}

		USEItemData* ItemData = *It;
		UGridSlot* GridSlot = ItemsPanel->AddChildToGrid(InventorySlotWidget);
		GridSlot->SetPadding(SlotsGridPanel->GetSlotPadding());

		InventorySlotWidget->SetItemData(ItemData);
		InventorySlotWidget->ParentWidget = this;

		InventoryItems.Add(InventorySlotWidget);
	}

	USEInventoryItemWidget* InventorySlotWidget = CreateWidget<USEInventoryItemWidget>(GetWorld(), InventoryItemWidgetClass);
	if (InventorySlotWidget != nullptr)
	{
		UGridSlot* GridSlot = ItemsPanel->AddChildToGrid(InventorySlotWidget);
		GridSlot->SetPadding(SlotsGridPanel->GetSlotPadding());

		GridSlot->SetRow(CharacterInventory->GetMaxSize().Y - 1);
		GridSlot->SetColumn(CharacterInventory->GetMaxSize().X - 1);

		GridSlot->SetRowSpan(1);
		GridSlot->SetColumnSpan(1);
		InventorySlotWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USEInventoryWidget::HideHealth()
{
	if (CurrentHealth != nullptr)
	{
		CurrentHealth->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USEInventoryWidget::UpdateHealth()
{
	if (CurrentHealth != nullptr)
	{
		CurrentHealth->SetVisibility(ESlateVisibility::Visible);

		const APawn* Character = GetOwningPlayerPawn();
		if (Character == nullptr)
		{
			return;
		}

		USEHealthComponent* CharacterHealth = Character->GetComponentByClass<USEHealthComponent>();
		if (CharacterHealth == nullptr)
		{
			return;
		}

		CurrentHealth->SetText(FText::FromString(FString::Printf(TEXT("Health: %.2f/%.2f"), CharacterHealth->GetCurrentHealth(), CharacterHealth->GetMaxHealtht())));
	}
}
