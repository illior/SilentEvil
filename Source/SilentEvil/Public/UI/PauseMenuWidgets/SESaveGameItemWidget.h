// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SEBaseButtonWidget.h"
#include "SESaveGameItemWidget.generated.h"

class USESaveGameWidget;

UCLASS()
class SILENTEVIL_API USESaveGameItemWidget : public USEBaseButtonWidget
{
	GENERATED_BODY()
	

public:
	USESaveGameWidget* MenuWidget;

	void SetSlotName(FString InName);
	FString GetSlotName();

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
	void SaveLevel();
};
