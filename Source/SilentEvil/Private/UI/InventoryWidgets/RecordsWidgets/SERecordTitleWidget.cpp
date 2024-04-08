// Silent Evil Game, All Rights Reserved.

#include "UI/InventoryWidgets/RecordsWidgets/SERecordTitleWidget.h"

#include "Components/TextBlock.h"

void USERecordTitleWidget::SetRecord(FSEReadableRecord NewRecord)
{
	Record = NewRecord;

	Text = Record.Title;
	//ButtonText->SetText(Record.Title);
}

FSEReadableRecord USERecordTitleWidget::GetRecord() const
{
	return Record;
}
