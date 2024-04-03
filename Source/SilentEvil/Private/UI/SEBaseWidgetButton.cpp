// Silent Evil Game, All Rights Reserved.


#include "UI/SEBaseWidgetButton.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void USEBaseWidgetButton::SetState(ESEButtonState NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}

	CurrentState = NewState;

	switch (CurrentState)
	{
	case ESEButtonState::Normal:
		ButtonImage->SetBrush(NormalBrush);
		break;
	case ESEButtonState::Hovered:
		ButtonImage->SetBrush(HoveredBrush);
		break;
	case ESEButtonState::Pressed:
		ButtonImage->SetBrush(PressedBrush);
		break;
	case ESEButtonState::Disabled:
		ButtonImage->SetBrush(DisabledBrush);
		break;
	default:
		break;
	}
}

ESEButtonState USEBaseWidgetButton::GetCurrentState() const
{
	return CurrentState;
}
