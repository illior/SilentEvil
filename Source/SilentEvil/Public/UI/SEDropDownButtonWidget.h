// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SECoreTypes.h"
#include "SEDropDownButtonWidget.generated.h"

class UImage;
struct FSlateBrush;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDropDownButtonClickedSignature);

UCLASS()
class SILENTEVIL_API USEDropDownButtonWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnDropDownButtonClickedSignature OnClicked;

	UFUNCTION(BlueprintCallable, Category = "Style")
	virtual void SetNormalBrush(const FSlateBrush& InBrush);

	UFUNCTION(BlueprintCallable, Category = "Style")
	const FSlateBrush& GetNormalBrush() const;

	UFUNCTION(BlueprintCallable, Category = "Style")
	virtual void SetHoverBrush(const FSlateBrush& InBrush);

	UFUNCTION(BlueprintCallable, Category = "Style")
	const FSlateBrush& GetHoverBrush() const;

	UFUNCTION(BlueprintCallable, Category = "Style")
	virtual void SetDisableBrush(const FSlateBrush& InBrush);

	UFUNCTION(BlueprintCallable, Category = "Style")
	const FSlateBrush& GetDisableBrush() const;

	UFUNCTION(BlueprintCallable, Category = "Style")
	EButtonState GetCurrentState() const;

	void SetNormal();

	void SetHover();

	void SetDisable();

	void Click();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, BlueprintSetter = "SetNormalBrush", Category = "Style")
	FSlateBrush NormalBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, BlueprintSetter = "SetHoverBrush", Category = "Style")
	FSlateBrush HoverBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, BlueprintSetter = "SetDisableBrush", Category = "Style")
	FSlateBrush DisableBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* BackgroundImage;

private:
	EButtonState CurrentState = EButtonState::Normal;
};
