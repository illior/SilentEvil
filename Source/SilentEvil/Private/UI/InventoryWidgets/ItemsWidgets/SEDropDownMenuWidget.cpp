// Silent Evil Game, All Rights Reserved.

#include "UI/InventoryWidgets/ItemsWidgets/SEDropDownMenuWidget.h"

#include "InventorySystem/SEItemData.h"
#include "InventorySystem/SEBaseItem.h"
#include "InventorySystem/Interfaces/SEUsableItemInterface.h"

DEFINE_LOG_CATEGORY_STATIC(LogDropDownMenu, All, All);

void USEDropDownMenuWidget::Open(USEItemData* ItemData, bool ForInteraction)
{
	SetVisibility(ESlateVisibility::Visible);

	for (USEDropDownButtonWidget* Button : Buttons)
	{
		Button->SetVisibility(ESlateVisibility::Collapsed);
		Button->SetDisable();
	}

	if (ForInteraction)
	{
		ExploreButton->SetVisibility(ESlateVisibility::Visible);
		ExploreButton->SetNormal();

		UseButton->SetVisibility(ESlateVisibility::Visible);
		UseButton->SetNormal();

		CombineButton->SetVisibility(ESlateVisibility::Visible);
		CombineButton->SetNormal();
	}
	else
	{
		ExploreButton->SetVisibility(ESlateVisibility::Visible);
		ExploreButton->SetNormal();

		CombineButton->SetVisibility(ESlateVisibility::Visible);
		CombineButton->SetNormal();

		DropButton->SetVisibility(ESlateVisibility::Visible);
		if (ItemData->CanDrop())
		{
			DropButton->SetNormal();
		}

		if (ItemData->GetItem()->Implements<USEUsableItemInterface>())
		{
			ISEUsableItemInterface* UsableItem = Cast<ISEUsableItemInterface>(ItemData->GetItem());

			UseButton->SetVisibility(ESlateVisibility::Visible);
			if (UsableItem->CanUse(GetOwningPlayerPawn()))
			{
				UseButton->SetNormal();
			}
			else
			{
				UseButton->SetDisable();
			}

		}

		if (ItemData->IsWeapon())
		{
			EquipButton->SetVisibility(ESlateVisibility::Visible);
			EquipButton->SetNormal();

			FastAccessButton->SetVisibility(ESlateVisibility::Visible);
			FastAccessButton->SetNormal();

			DropButton->SetVisibility(ESlateVisibility::Collapsed);
			DropButton->SetDisable();
		}
	}

	CurrentIndex = 0;
	Buttons[CurrentIndex]->SetHover();
}

void USEDropDownMenuWidget::Close()
{
	SetVisibility(ESlateVisibility::Hidden);
}

bool USEDropDownMenuWidget::IsActive()
{
	return GetVisibility() == ESlateVisibility::Visible;
}

void USEDropDownMenuWidget::MoveSelectedUp()
{
	if (Buttons[CurrentIndex]->GetCurrentState() != EButtonState::Disable)
	{
		Buttons[CurrentIndex]->SetNormal();
	}

	CurrentIndex = (CurrentIndex - 1) < 0 ? Buttons.Num() - 1 : CurrentIndex - 1;

	if (Buttons[CurrentIndex]->GetCurrentState() == EButtonState::Disable)
	{
		MoveSelectedUp();
		return;
	}

	Buttons[CurrentIndex]->SetHover();
}

void USEDropDownMenuWidget::MoveSelectedDown()
{
	if (Buttons[CurrentIndex]->GetCurrentState() != EButtonState::Disable)
	{
		Buttons[CurrentIndex]->SetNormal();
	}

	CurrentIndex = (CurrentIndex + 1) % Buttons.Num();

	if (Buttons[CurrentIndex]->GetCurrentState() == EButtonState::Disable)
	{
		MoveSelectedDown();
		return;
	}

	Buttons[CurrentIndex]->SetHover();
}

void USEDropDownMenuWidget::ActivateSelected()
{
	if(Buttons[CurrentIndex] == nullptr)
	{
		return;
	}

	Buttons[CurrentIndex]->Click();
}

FOnDropDownButtonClickedSignature& USEDropDownMenuWidget::GetExploreButtonSignature()
{
	return ExploreButton->OnClicked;
}

FOnDropDownButtonClickedSignature& USEDropDownMenuWidget::GetUseButtonSignature()
{
	return UseButton->OnClicked;
}

FOnDropDownButtonClickedSignature& USEDropDownMenuWidget::GetEquipButtonSignature()
{
	return EquipButton->OnClicked;
}

FOnDropDownButtonClickedSignature& USEDropDownMenuWidget::GetFastAccessButtonSignature()
{
	return FastAccessButton->OnClicked;
}

FOnDropDownButtonClickedSignature& USEDropDownMenuWidget::GetCombineButtonSignature()
{
	return CombineButton->OnClicked;
}

FOnDropDownButtonClickedSignature& USEDropDownMenuWidget::GetDropButtonSignature()
{
	return DropButton->OnClicked;
}

void USEDropDownMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Buttons.Add(ExploreButton);
	Buttons.Add(UseButton);
	Buttons.Add(EquipButton);
	Buttons.Add(FastAccessButton);
	Buttons.Add(CombineButton);
	Buttons.Add(DropButton);
}
