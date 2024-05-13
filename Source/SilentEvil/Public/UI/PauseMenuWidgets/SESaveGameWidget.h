// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SESubmenuWidget.h"
#include "SESaveGameWidget.generated.h"

class UScrollBox;

class USESaveGameItemWidget;

UCLASS()
class SILENTEVIL_API USESaveGameWidget : public USESubmenuWidget
{
	GENERATED_BODY()
	

public:
	virtual void Open() override;
	virtual void Close() override;

	virtual void MoveVertical(int32 Offset) override;
	virtual void Apply() override;
	virtual bool Cancel() override;

	void SetSelected(USESaveGameItemWidget* InButton);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<USESaveGameItemWidget> SaveGameItemWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	int32 DefaultSavesCount = 5;

	UPROPERTY()
	TArray<USESaveGameItemWidget*> SaveSlots;
	int32 CurrentSlotIndex = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FMargin SaveSlotPadding;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* Scroll;

	virtual void NativeOnInitialized() override;

private:

};
