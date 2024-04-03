// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SEBaseWidgetButton.h"
#include "InventorySystem/SEReadableRecord.h"
#include "SERecordTitleWidget.generated.h"

class UTextBlock;

UCLASS()
class SILENTEVIL_API USERecordTitleWidget : public USEBaseWidgetButton
{
	GENERATED_BODY()

public:
	void SetRecord(FSEReadableRecord NewRecord);
	FSEReadableRecord GetRecord() const;

protected:
	FSEReadableRecord Record;

};
