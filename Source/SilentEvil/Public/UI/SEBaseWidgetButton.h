// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SECoreTypes.h"
#include "SEBaseWidgetButton.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class SILENTEVIL_API USEBaseWidgetButton : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void SetState(ESEButtonState NewState);
	ESEButtonState GetCurrentState() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FSlateBrush NormalBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FSlateBrush HoveredBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FSlateBrush PressedBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FSlateBrush DisabledBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Style")
	FSlateColor TextColorAndOpacity = FSlateColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Style")
	FSlateFontInfo TextFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Style")
	FSlateBrush TextStrikeBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Style")
	FVector2D TextShadowOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Style")
	FLinearColor TextShadowColorAndOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
	FText Text = FText();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ButtonImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ButtonText;

private:
	ESEButtonState CurrentState = ESEButtonState::Normal;
};
