// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/InventoryWidgets/SEBaseWidget.h"
#include "InventorySystem/SEReadableRecord.h"
#include "SERecordsWidget.generated.h"

class UImage;
class UTextBlock;
class UScrollBox;
class USERecordTitleWidget;

UCLASS()
class SILENTEVIL_API USERecordsWidget : public USEBaseWidget
{
	GENERATED_BODY()


public:
	void UpdataData();

	virtual bool Cancel() override;
	virtual void MoveVertical(float Value) override;
	virtual void MoveHorizontal(float Value) override;
	virtual void Apply() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FMargin RecordPadding = FMargin(0.0f, 0.0f, 0.0f, 5.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<USERecordTitleWidget> RecordWidgetClass;

	UPROPERTY()
	TArray<USERecordTitleWidget*> Records;

	int32 CurrentIndex;

	int32 CurrentPage;
	FSEReadableRecord CurrentRecord;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* RecordTitles;

	UPROPERTY(meta = (BindWidget))
	UImage* LeftArrow;
	UPROPERTY(meta = (BindWidget))
	UImage* RightArrow;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RecordText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Pages;

private:
	void CreateRecords();

	void ShowPages();
};
