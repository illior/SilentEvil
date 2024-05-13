// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SEBaseButtonWidget.h"
#include "SELoadGameItemWidget.generated.h"

class USELoadGameWidget;

UCLASS()
class SILENTEVIL_API USELoadGameItemWidget : public USEBaseButtonWidget
{
	GENERATED_BODY()
	

public:
	USELoadGameWidget* MenuWidget;

	void SetSlotName(FString InName);
	FString GetSlotName();

	void UpdateData();

	void Click();

protected:
	UPROPERTY(BlueprintReadWrite)
	FString SlotName = "DefaultSlot";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText NoDataMessage = FText::FromString("No Data");

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SaveDate;

	virtual void NativeOnInitialized() override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	UFUNCTION()
	void OpenLevel();
};
