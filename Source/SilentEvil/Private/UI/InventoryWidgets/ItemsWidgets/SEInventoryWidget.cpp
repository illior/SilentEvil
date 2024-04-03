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

#include "Player/SEPlayerController.h"
#include "Components/SEInventoryComponent.h"
#include "Components/SEHealthComponent.h"
#include "InventorySystem/SEItemData.h"
#include "InventorySystem/SEWeaponData.h"
#include "InventorySystem/Interfaces/SEUsableItemInterface.h"
#include "InventorySystem/Actors/SEItemVisual.h"
#include "InteractableObjects/SEInteractableTarget.h"

DEFINE_LOG_CATEGORY_STATIC(LogUInventory, All, All);

bool USEInventoryWidget::Cancel()
{
	if (ListenForBind)
	{
		StopFastAccess();

		return false;
	}

	if (DropDownMenuIsOpen)
	{
		CloseDropDownMenu();

		return false;
	}

	if (ItemIsMoving)
	{
		CancelMoveItem();

		return false;
	}

	if (ItemInspect)
	{
		if (CurrentItemWidget->GetItemData()->GetItemVisual()->Cancel())
		{
			StopInspectItem();
		}

		return false;
	}

	return true;
}

void USEInventoryWidget::MoveItem()
{
	if (ListenForBind)
	{
		StopFastAccess();
		return;
	}

	if (ItemIsMoving)
	{
		StopMoveItem();
		return;
	}

	if (ItemInspect)
	{
		// item inspect
		return;
	}

	if (!DropDownMenuIsOpen)
	{
		StartMoveItem();
	}
}

void USEInventoryWidget::MoveVertical(float Value)
{
	if (ListenForBind)
	{
		StopFastAccess();
		return;
	}

	if (DropDownMenu == nullptr)
	{
		return;
	}

	if (DropDownMenuIsOpen)
	{
		Value > 0 ? DropDownMenu->MoveSelectedUp() : DropDownMenu->MoveSelectedDown();

		return;
	}

	if (ItemInspect)
	{
		CurrentItemWidget->GetItemData()->GetItemVisual()->VerticalInput(Value);

		return;
	}

	MoveItemTo(FIntPoint(0, (int32)Value));
}

void USEInventoryWidget::MoveHorizontal(float Value)
{
	if (ListenForBind)
	{
		StopFastAccess();
		return;
	}

	if (DropDownMenuIsOpen)
	{
		CloseDropDownMenu();

		return;
	}

	if (ItemInspect)
	{
		CurrentItemWidget->GetItemData()->GetItemVisual()->HorizontalInput(Value);

		return;
	}

	MoveItemTo(FIntPoint((int32)Value, 0));
}

void USEInventoryWidget::FastAccessItem(float Value)
{
	if (ListenForBind)
	{
		SetFastAccess((int32)Value - 1);
	}
}

void USEInventoryWidget::Apply()
{
	if (ListenForBind)
	{
		StopFastAccess();
		return;
	}

	if (DropDownMenu == nullptr)
	{
		return;
	}

	if (ItemIsMoving)
	{
		StopMoveItem();

		return;
	}

	if (DropDownMenuIsOpen)
	{
		DropDownMenu->ActivateSelected();

		return;
	}

	if (ItemInspect)
	{
		CurrentItemWidget->GetItemData()->GetItemVisual()->Apply();

		return;
	}

	OpenDropDownMenu();
}

void USEInventoryWidget::UpdateInventoryData(bool OpenForInteraction)
{
	ForInteraction = OpenForInteraction;
	CurrentItemWidget = nullptr;

	InspectPanel->SetVisibility(ESlateVisibility::Collapsed);
	ItemInspect = false;

	UpdateSlots();
	UpdateItems();

	if (ForInteraction)
	{
		HideFastAccessItems();
		HideHealth();
	}
	else
	{
		UpdateFastAccessItems();
		UpdateHealth();
	}
	

	SetCurrentSelected();
	InventoryCursor->SetViewState(EViewState::Normal);

	CloseDropDownMenu();

	ItemIsMoving = false;
	DropDownMenuIsOpen = false;
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
	DropDownMenu->GetCombineButtonSignature().AddDynamic(this, &USEInventoryWidget::StartMoveItem);
	DropDownMenu->GetDropButtonSignature().AddDynamic(this, &USEInventoryWidget::DropItem);

	USEInventoryComponent* CharacterInventory = GetCharacterInventory();
	if (CharacterInventory != nullptr)
	{
		CharacterInventory->OnItemCrafted.AddDynamic(this, &USEInventoryWidget::OnItemCrafted);
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

	if (!InventorySlots.Contains(NewPosition) || InventorySlots[NewPosition]->GetState() == ESlotState::Block)
	{
		return;
	}

	if (ItemIsMoving)
	{
		InventoryCursor->SetPosition(NewPosition);
	}
	else
	{
		SetCurrentUnSelected();
		InventoryCursor->SetPosition(NewPosition);
		SetCurrentSelected();
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

			if (ItemName != nullptr)
			{
				ItemName->SetText(FText::FromName(CurrentItemWidget->GetItemData()->GetItemName()));
			}
		}
	}
	else
	{
		CurrentItemWidget = nullptr;
		InventorySlots[InventoryCursor->GetPosition()]->SetViewState(EViewState::Selected);

		if (ItemName != nullptr)
		{
			ItemName->SetText(FText::FromString(""));
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

	ListenForBind = false;
	UE_LOG(LogUInventory, Display, TEXT("FastAccess %i"), Index);
}

void USEInventoryWidget::StopFastAccess()
{
	ListenForBind = false;
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
			ItemIsMoving = false;

			UpdateSlots(); // Need fix
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
		ItemIsMoving = false;

		CurrentItemWidget->SetViewState(EViewState::Selected);
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
	ItemIsMoving = false;

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
	DropDownMenu->Open(CurrentItemWidget->GetItemData(), ForInteraction);
	DropDownMenuIsOpen = true;
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
	DropDownMenuIsOpen = false;
}

void USEInventoryWidget::StartInspectItem()
{
	ItemInspect = true;

	ASEItemVisual* ItemActor = CurrentItemWidget->GetItemData()->GetItemVisual();
	ItemActor->SetActorHiddenInGame(false);

	InspectPanel->SetVisibility(ESlateVisibility::Visible);
	UE_LOG(LogUInventory, Display, TEXT("StartInspectItem"));
}

void USEInventoryWidget::StopInspectItem()
{
	ItemInspect = false;

	ASEItemVisual* ItemActor = CurrentItemWidget->GetItemData()->GetItemVisual();
	ItemActor->SetActorHiddenInGame(true);

	InspectPanel->SetVisibility(ESlateVisibility::Collapsed);
	UE_LOG(LogUInventory, Display, TEXT("StopInspectItem"));
}

void USEInventoryWidget::StartMoveItem()
{
	if (InventoryCursor == nullptr || CurrentItemWidget == nullptr)
	{
		return;
	}

	if (DropDownMenuIsOpen)
	{
		CloseDropDownMenu();
	}

	ItemIsMoving = true;
	InventoryCursor->SetViewState(EViewState::Active);
	CurrentItemWidget->SetViewState(EViewState::Active);
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

	if (ForInteraction)
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

		if (TargetObject->TryUseItem(CurrentItemWidget->GetItemData()->GetItem()))
		{
			if (TargetObject->GetRemoveItem())
			{
				DropItem();
			}
		}
	}
	else
	{
		USEInventoryComponent* CharacterInventory = GetCharacterInventory();
		if (CharacterInventory == nullptr)
		{
			return;
		}

		CharacterInventory->UseItem(CurrentItemWidget->GetItemData());

		UpdateHealth();
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
	ListenForBind = true;

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

void USEInventoryWidget::OnItemCrafted(USEItemData* NewItemData)
{
	if (ItemsPanel == nullptr || SlotsGridPanel == nullptr)
	{
		return;
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

	UpdateSlots(); // Need fix
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
