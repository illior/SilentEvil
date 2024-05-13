// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SESubmenuWidget.h"
#include "SELoadGameWidget.generated.h"

class UScrollBox;

class USELoadGameItemWidget;

UCLASS()
class SILENTEVIL_API USELoadGameWidget : public USESubmenuWidget
{
	GENERATED_BODY()
	

public:
	virtual void Open();

	virtual void MoveVertical(int32 Offset) override;
	virtual void Apply() override;
	virtual bool Cancel() override;

	void SetSelected(USELoadGameItemWidget* InButton);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<USELoadGameItemWidget> LoadGameItemWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	int32 DefaultSavesCount = 5;

	UPROPERTY()
	TArray<USELoadGameItemWidget*> SaveSlots;
	int32 CurrentSlotIndex = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FMargin SaveSlotPadding;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* Scroll;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Hide;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Show;

	virtual void NativeOnInitialized() override;

private:
	FWidgetAnimationDynamicEvent ShowMenuDelegate;

	UFUNCTION()
	void ActuallyOpenMenu();
};
