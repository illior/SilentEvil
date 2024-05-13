// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SEBaseButtonWidget.h"
#include "SEMenuButtonWidget.generated.h"

class UWidgetAnimation;

class USEBaseMenuWidget;

UCLASS()
class SILENTEVIL_API USEMenuButtonWidget : public USEBaseButtonWidget
{
	GENERATED_BODY()
	

public:
	USEBaseMenuWidget* MenuWidget;

	void Click();

protected:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void SetHovered(ESEButtonState OldState) override;
	virtual void SetNormal(ESEButtonState OldState) override;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Focus;
};
