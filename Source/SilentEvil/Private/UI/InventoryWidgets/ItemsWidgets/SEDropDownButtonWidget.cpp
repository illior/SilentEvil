// Silent Evil Game, All Rights Reserved.


#include "UI/InventoryWidgets/ItemsWidgets/SEDropDownButtonWidget.h"

#include "Styling/SlateBrush.h"
#include "Components/Image.h"

void USEDropDownButtonWidget::SetNormalBrush(const FSlateBrush& InBrush)
{
	if (NormalBrush != InBrush)
	{
		NormalBrush = InBrush;
	}
}

const FSlateBrush& USEDropDownButtonWidget::GetNormalBrush() const
{
	return NormalBrush;
}

void USEDropDownButtonWidget::SetHoverBrush(const FSlateBrush& InBrush)
{
	if (NormalBrush != InBrush)
	{
		HoverBrush = InBrush;
	}
}

const FSlateBrush& USEDropDownButtonWidget::GetHoverBrush() const
{
	return HoverBrush;
}

void USEDropDownButtonWidget::SetDisableBrush(const FSlateBrush& InBrush)
{
	if (NormalBrush != InBrush)
	{
		DisableBrush = InBrush;
	}
}

const FSlateBrush& USEDropDownButtonWidget::GetDisableBrush() const
{
	return DisableBrush;
}

EButtonState USEDropDownButtonWidget::GetCurrentState() const
{
	return CurrentState;
}

void USEDropDownButtonWidget::SetNormal()
{
	if (CurrentState == EButtonState::Normal || BackgroundImage == nullptr)
	{
		return;
	}

	CurrentState = EButtonState::Normal;
	BackgroundImage->SetBrush(NormalBrush);
}

void USEDropDownButtonWidget::SetHover()
{
	if (CurrentState == EButtonState::Hover || BackgroundImage == nullptr)
	{
		return;
	}
	
	CurrentState = EButtonState::Hover;
	BackgroundImage->SetBrush(HoverBrush);
}

void USEDropDownButtonWidget::SetDisable()
{
	if (CurrentState == EButtonState::Disable || BackgroundImage == nullptr)
	{
		return;
	}

	CurrentState = EButtonState::Disable;
	BackgroundImage->SetBrush(DisableBrush);
}

void USEDropDownButtonWidget::Click()
{
	OnClicked.Broadcast();
}
