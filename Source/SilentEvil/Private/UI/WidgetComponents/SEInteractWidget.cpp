// Silent Evil Game, All Rights Reserved.

#include "UI/WidgetComponents/SEInteractWidget.h"

#include "Components/SizeBox.h"

void USEInteractWidget::SetVisibleKey(bool NewValue)
{
	if (NewValue)
	{
		KeyWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		KeyWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool USEInteractWidget::GetVisibleKey()
{
	switch (KeyWidget->GetVisibility())
	{
	case ESlateVisibility::Visible:
		return true;
	case ESlateVisibility::Hidden:
		return false;
	}

	return false;
}
