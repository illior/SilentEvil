// Silent Evil Game, All Rights Reserved.

#include "UI/InventoryWidgets/ItemsWidgets/SEInventorySlotWidget.h"

#include "Components/Image.h"

void USEInventorySlotWidget::SetState(ESlotState NewState)
{
	State = NewState;

	if (State == ESlotState::Block)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);
	}
}

ESlotState USEInventorySlotWidget::GetState()
{
	return State;
}

void USEInventorySlotWidget::SetViewState(EViewState NewState)
{
	if (SelectedImage == nullptr || NewState == CurrentViewState)
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
	default:
		SelectedImage->SetColorAndOpacity(NormalColor);
		break;
	}
}

EViewState USEInventorySlotWidget::GetViewState()
{
	return CurrentViewState;
}
