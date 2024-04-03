// Silent Evil Game, All Rights Reserved.

#include "UI/InventoryWidgets/ItemsWidgets/SEInventoryCursorWidget.h"

#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Animation/WidgetAnimation.h"

void USEInventoryCursorWidget::SetMargin(const FMargin& NewMargin)
{
	Margin = NewMargin;

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
	if (CanvasSlot != nullptr)
	{
		CanvasSlot->SetPosition(FVector2D((CanvasSlot->GetSize().X + (Margin.Left + Margin.Right)) * Position.X,
			(CanvasSlot->GetSize().Y + (Margin.Top + Margin.Bottom)) * Position.Y));
	}
}

void USEInventoryCursorWidget::SetPosition(FIntPoint NewPosition)
{
	Position = NewPosition;

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
	if (CanvasSlot != nullptr)
	{
		CanvasSlot->SetPosition(FVector2D((CanvasSlot->GetSize().X + (Margin.Left + Margin.Right)) * Position.X,
			(CanvasSlot->GetSize().Y + (Margin.Top + Margin.Bottom)) * Position.Y));
	}
}

FIntPoint USEInventoryCursorWidget::GetPosition()
{
	return Position;
}

void USEInventoryCursorWidget::SetViewState(EViewState NewState)
{
	if (SelectedImage == nullptr || NewState == CurrentViewState)
	{
		return;
	}

	CurrentViewState = NewState;
	switch (CurrentViewState)
	{
	case EViewState::Normal:
		StopAnimation(CursorActive);
		break;
	case EViewState::Active:
		PlayAnimation(CursorActive, 0.0f, 0, EUMGSequencePlayMode::PingPong, 1.0f, true);
		break;
	default:
		StopAnimation(CursorActive);
		break;
	}
}

EViewState USEInventoryCursorWidget::GetViewState()
{
	return CurrentViewState;
}
