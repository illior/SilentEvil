// Silent Evil Game, All Rights Reserved.

#include "UI/SERecordTitleWidget.h"

#include "Components/TextBlock.h"

void USERecordTitleWidget::SetRecord(FSEReadableRecord NewRecord)
{
	Record = NewRecord;

	ButtonText->SetText(FText::FromName(Record.Title));
}

FSEReadableRecord USERecordTitleWidget::GetRecord() const
{
	return Record;
}
