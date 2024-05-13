// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SECoreTypes.h"
#include "SEBaseButtonWidget.generated.h"

class UImage;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSEOnButtonClickSignature);

UCLASS()
class SILENTEVIL_API USEBaseButtonWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FSEOnButtonClickSignature OnClicked;

	void SetState(ESEButtonState NewState);
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
	FText Text = FText();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ButtonImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ButtonText;

	virtual void NativePreConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual void SetNormal(ESEButtonState OldState);
	virtual void SetHovered(ESEButtonState OldState);
	virtual void SetPressed(ESEButtonState OldState);
	virtual void SetDisabled(ESEButtonState OldState);

private:
	ESEButtonState CurrentState = ESEButtonState::Normal;
};
