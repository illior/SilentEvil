// Silent Evil Game, All Rights Reserved.

#include "UI/InventoryWidgets/ItemsWidgets/SEStorageButtonWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventorySystem/SEItemData.h"

void USEStorageButtonWidget::SetItemData(USEItemData* InItemData)
{
	if (InItemData == nullptr)
	{
		SetVisibility(ESlateVisibility::Collapsed);

		if (ItemData != nullptr)
		{
			ItemData->OnDataChanged.RemoveDynamic(this, &USEStorageButtonWidget::UpdateCount);
			ItemData = nullptr;
		}

		return;
	}

	ItemData = InItemData;

	ItemData->OnDataChanged.AddDynamic(this, &USEStorageButtonWidget::UpdateCount);

	ItemImage->SetBrushFromTexture(ItemData->GetItemIcon());
	ButtonText->SetText(ItemData->GetItemName());

	ItemCount->SetText(FText::FromString(FString::FromInt(ItemData->GetCount())));
}

USEItemData* USEStorageButtonWidget::GetItemData()
{
	return ItemData;
}

void USEStorageButtonWidget::UpdateCount()
{
	ItemCount->SetText(FText::FromString(FString::FromInt(ItemData->GetCount())));
}
