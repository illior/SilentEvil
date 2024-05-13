// Silent Evil Game, All Rights Reserved.

#include "UI/MainMenuWidgets/SEMenuButtonWidget.h"

#include "UI/SEBaseMenuWidget.h"

#include "Animation/WidgetAnimation.h"

void USEMenuButtonWidget::Click()
{
	OnClicked.Broadcast();
}

void USEMenuButtonWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (MenuWidget != nullptr)
	{
		MenuWidget->SetSelected(this);
	}

	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void USEMenuButtonWidget::SetHovered(ESEButtonState OldState)
{
	if (OldState != ESEButtonState::Pressed)
	{
		if (IsAnimationPlaying(Focus))
		{
			float TimeToStart = GetAnimationCurrentTime(Focus);

			PlayAnimation(Focus, TimeToStart);
		}
		else
		{
			PlayAnimation(Focus);
		}
	}

	Super::SetHovered(OldState);
}

void USEMenuButtonWidget::SetNormal(ESEButtonState OldState)
{
	if (IsAnimationPlaying(Focus))
	{
		float TimeToStart = Focus->GetEndTime() - GetAnimationCurrentTime(Focus);

		PlayAnimation(Focus, TimeToStart, 1, EUMGSequencePlayMode::Reverse);
	}
	else
	{
		PlayAnimation(Focus, 0, 1, EUMGSequencePlayMode::Reverse);
	}

	Super::SetNormal(OldState);
}
