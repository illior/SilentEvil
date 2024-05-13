// Silent Evil Game, All Rights Reserved.


#include "UI/SESubmenuWidget.h"

void USESubmenuWidget::Open()
{
	SetVisibility(ESlateVisibility::Visible);
}

void USESubmenuWidget::Close()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void USESubmenuWidget::MoveVertical(int32 Offset)
{
}

void USESubmenuWidget::MoveHorizontal(int32 Offset)
{
}

void USESubmenuWidget::MoveAdditiveHorizotnal(int32 Offset)
{
}

void USESubmenuWidget::Apply()
{
}

bool USESubmenuWidget::Cancel()
{
	return true;
}
