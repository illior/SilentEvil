// Silent Evil Game, All Rights Reserved.

#include "UI/SEBaseButtonWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

DEFINE_LOG_CATEGORY_STATIC(LogSEBaseButton, All, All);

void USEBaseButtonWidget::SetState(ESEButtonState NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}

	ESEButtonState OldState = CurrentState;
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ESEButtonState::Normal:
		SetNormal(OldState);
		break;
	case ESEButtonState::Hovered:
		SetHovered(OldState);
		break;
	case ESEButtonState::Pressed:
		SetPressed(OldState);
		break;
	case ESEButtonState::Disabled:
		SetDisabled(OldState);
		break;
	default:
		break;
	}
}

ESEButtonState USEBaseButtonWidget::GetCurrentState() const
{
	return CurrentState;
}

void USEBaseButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (ButtonText != nullptr)
	{
		ButtonText->SetText(Text);
	}
	
	if (ButtonImage != nullptr)
	{
		ButtonImage->SetBrush(NormalBrush);
	}
}

FReply USEBaseButtonWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == FKey(FName("LeftMouseButton")))
	{
		SetState(ESEButtonState::Pressed);
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply USEBaseButtonWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (GetCurrentState() == ESEButtonState::Pressed && InMouseEvent.GetEffectingButton() == FKey(FName("LeftMouseButton")))
	{
		OnClicked.Broadcast();
		SetState(ESEButtonState::Hovered);
	}

	return FReply::Unhandled();
}

void USEBaseButtonWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	SetState(ESEButtonState::Hovered);
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void USEBaseButtonWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	SetState(ESEButtonState::Normal);
	Super::NativeOnMouseLeave(InMouseEvent);
}

void USEBaseButtonWidget::SetNormal(ESEButtonState OldState)
{
	ButtonImage->SetBrush(NormalBrush);
}

void USEBaseButtonWidget::SetHovered(ESEButtonState OldState)
{
	ButtonImage->SetBrush(HoveredBrush);
}

void USEBaseButtonWidget::SetPressed(ESEButtonState OldState)
{
	ButtonImage->SetBrush(PressedBrush);
}

void USEBaseButtonWidget::SetDisabled(ESEButtonState OldState)
{
	ButtonImage->SetBrush(DisabledBrush);
}
