// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SECoreTypes.h"
#include "SEInventorySlotWidget.generated.h"

class UImage;

UCLASS()
class SILENTEVIL_API USEInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	void SetState(ESlotState NewState);
	ESlotState GetState();

	void SetViewState(EViewState NewState);
	EViewState GetViewState();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* SelectedImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Appearance")
	FLinearColor NormalColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Appearance")
	FLinearColor SelectedColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.5f);

private:
	EViewState CurrentViewState = EViewState::Normal;
	ESlotState State = ESlotState::Void;
};
