// Silent Evil Game, All Rights Reserved.

#include "UI/InventoryWidgets/ItemsWidgets/SEInventoryItemWidget.h"

#include "Components/GridSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "UI/InventoryWidgets/ItemsWidgets/SEInventoryWidget.h"
#include "InventorySystem/SEItemData.h"
#include "InventorySystem/SEWeaponData.h"
#include "InventorySystem/SEBaseItem.h"

DEFINE_LOG_CATEGORY_STATIC(LogUIInventoryItem, All, All);

void USEInventoryItemWidget::SetItemData(USEItemData* NewItemData)
{
	if (NewItemData == nullptr)
	{
		SetEmptyItem();

		return;
	}

	ItemData = NewItemData;
	UpdateData();

	ItemData->OnDataChanged.AddDynamic(this, &USEInventoryItemWidget::OnDataChanged);
	ItemData->OnDrop.AddDynamic(this, &USEInventoryItemWidget::OnDropItem);

	Place();
}

USEItemData* USEInventoryItemWidget::GetItemData()
{
	return ItemData;
}

void USEInventoryItemWidget::SetViewState(EViewState NewState)
{
	if (SelectedImage == nullptr)
	{
		return;
	}

	CurrentViewState = NewState;
	switch (CurrentViewState)
	{
	case EViewState::Normal:
		SelectedImage->SetColorAndOpacity(NormalColor);
		break;
	case EViewState::Selected:
		SelectedImage->SetColorAndOpacity(SelectedColor);
		break;
	case EViewState::Active:
		SelectedImage->SetColorAndOpacity(ActiveColor);
		break;
	default:
		SelectedImage->SetColorAndOpacity(NormalColor);
		break;
	}
}

EViewState USEInventoryItemWidget::GetViewState()
{
	return CurrentViewState;
}

void USEInventoryItemWidget::OnDataChanged()
{
	UpdateData();
	UpdatePosition();
}

void USEInventoryItemWidget::OnDropItem()
{
	if (ItemData != nullptr)
	{
		ItemData->OnDataChanged.RemoveDynamic(this, &USEInventoryItemWidget::OnDataChanged);

		ItemData->OnDrop.RemoveDynamic(this, &USEInventoryItemWidget::OnDropItem);
	}

	if (ParentWidget != nullptr)
	{
		ParentWidget->RemoveItem(this);
	}

	ItemImage->SetBrush(EmptyItem);
	CountText->SetText(FText());
	ItemData = nullptr;

	SetVisibility(ESlateVisibility::Collapsed);
}

void USEInventoryItemWidget::Place()
{
	if (ItemData == nullptr)
	{
		return;
	}

	UGridSlot* GridSlot = Cast<UGridSlot>(Slot);
	if (GridSlot != nullptr)
	{
		GridSlot->SetRowSpan(1);
		GridSlot->SetColumnSpan(ItemData->GetItem()->GetSize());

		GridSlot->SetRow(ItemData->GetPosition().Y);
		GridSlot->SetColumn(ItemData->GetPosition().X);

		return;
	}

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
	if (CanvasSlot != nullptr)
	{
		CanvasSlot->SetSize(FVector2D(InitialWidth * ItemData->GetItemSize(), CanvasSlot->GetSize().Y));
	}
}

void USEInventoryItemWidget::SetEmptyItem()
{
	if (ItemImage == nullptr || CountText == nullptr || FastAccessText == nullptr || EquipText == nullptr)
	{
		return;
	}

	if (ItemData != nullptr)
	{
		ItemData->OnDataChanged.RemoveDynamic(this, &USEInventoryItemWidget::OnDataChanged);
		ItemData->OnDrop.RemoveDynamic(this, &USEInventoryItemWidget::OnDropItem);
	}

	ItemImage->SetBrush(EmptyItem);

	CountText->SetText(FText());
	FastAccessText->SetText(FText());
	EquipText->SetText(FText());

	ItemData = nullptr;

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
	if (CanvasSlot != nullptr)
	{
		CanvasSlot->SetSize(FVector2D(InitialWidth, CanvasSlot->GetSize().Y));
	}
}

void USEInventoryItemWidget::UpdateData()
{
	if (ItemImage == nullptr || CountText == nullptr || FastAccessText == nullptr || EquipText == nullptr)
	{
		return;
	}

	ItemImage->SetBrushFromTexture(ItemData->GetItem()->GetIcon());

	if (ItemData->IsWeapon())
	{
		USEWeaponData* WeaponData = Cast<USEWeaponData>(ItemData);
		if (WeaponData != nullptr)
		{
			CountText->SetText(FText::FromString(FString::FromInt(WeaponData->GetCurrentAmmoInClip())));
			CountText->SetColorAndOpacity(WeaponData->GetCurrentAmmoInClip() == WeaponData->GetMaxAmmoInClip() ?
				FullCountText : NotFullCountText);

			EquipText->SetText(FText::FromString(FString(WeaponData->GetIsEquip() ? TEXT("E") : FString())));
			FastAccessText->SetText(FText::FromString(WeaponData->GetFastAccessIndex() < 0 ? 
				FString() : FString::FromInt(WeaponData->GetFastAccessIndex() + 1)));
		}
	}
	else
	{
		CountText->SetText(ItemData->GetItemCanStack() ? FText::FromString(FString::FromInt(ItemData->GetCount())) : FText());
		CountText->SetColorAndOpacity(ItemData->GetCount() == ItemData->GetItemMaxStackAmount() ? 
			FullCountText : NotFullCountText);

		EquipText->SetText(FText());
		FastAccessText->SetText(FText());
	}
}

void USEInventoryItemWidget::UpdatePosition()
{
	UGridSlot* GridSlot = Cast<UGridSlot>(Slot);
	if (GridSlot == nullptr || ItemData == nullptr)
	{
		return;
	}

	GridSlot->SetRow(ItemData->GetPosition().Y);
	GridSlot->SetColumn(ItemData->GetPosition().X);
}
