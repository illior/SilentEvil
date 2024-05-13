// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SEBaseMenuWidget.h"
#include "SEPauseMenuWidget.generated.h"

class USEMenuButtonWidget;
class USESubmenuWidget;
class USELoadGameWidget;

class UInputAction;
struct FInputActionValue;

class UWidgetAnimation;

UCLASS()
class SILENTEVIL_API USEPauseMenuWidget : public USEBaseMenuWidget
{
	GENERATED_BODY()


public:
	UPROPERTY()
	USESubmenuWidget* CurrentTargetWidget = nullptr;

	virtual void SetSelected(USEBaseButtonWidget* InButton) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* CancelAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* VerticalMoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* ApplyAction;

	UPROPERTY()
	TArray<USEMenuButtonWidget*> Buttons;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USEMenuButtonWidget* ContinueButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USEMenuButtonWidget* LoadGameButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USEMenuButtonWidget* SettingsButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USEMenuButtonWidget* ExitButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USELoadGameWidget* LoadGameWidget;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* HideButtons;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ShowButtons;

	virtual void NativeOnInitialized() override;

private:
	FWidgetAnimationDynamicEvent ShowButtonsDelegate;

	int32 CurrentIndex = -1;

	UPROPERTY()
	USESubmenuWidget* CurrentSubmenu = nullptr;

	UFUNCTION()
	void Continue();

	UFUNCTION()
	void LoadGame();

	UFUNCTION()
	void Settings();

	UFUNCTION()
	void Exit();

	UFUNCTION()
	void CloseCurrentSubmenu();

	void MoveVertical(const FInputActionValue& Value);
	void Apply(const FInputActionValue& Value);
	void Cancel(const FInputActionValue& Value);

	void SetupInput();

	void Close();
};
