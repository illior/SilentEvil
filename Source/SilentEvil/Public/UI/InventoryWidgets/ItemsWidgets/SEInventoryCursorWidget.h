// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SECoreTypes.h"
#include "SEInventoryCursorWidget.generated.h"

class UImage;
class UWidgetAnimation;

UCLASS()
class SILENTEVIL_API USEInventoryCursorWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	FMargin Margin = FMargin(0.0f);
	void SetMargin(const FMargin& NewMargin);

	void SetPosition(FIntPoint NewPosition);
	FIntPoint GetPosition();

	void SetViewState(EViewState NewState);
	EViewState GetViewState();

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* SelectedImage;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* CursorActive;

private:
	EViewState CurrentViewState = EViewState::Normal;
	FIntPoint Position = FIntPoint(0, 0);
};
