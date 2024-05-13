// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SEBaseMenuWidget.h"
#include "SEMainMenuWidget.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

class USEBaseButtonWidget;
class USEMenuButtonWidget;
class USESubmenuWidget;
class USELoadGameWidget;

class UWidgetAnimation;

UCLASS()
class SILENTEVIL_API USEMainMenuWidget : public USEBaseMenuWidget
{
	GENERATED_BODY()
	

public:
	virtual void SetSelected(USEBaseButtonWidget* InButton) override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void SetupInput();

	bool IsMenuAnimationPlaying();

	UPROPERTY()
	TArray<USEMenuButtonWidget*> Buttons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* MainMenuMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* VerticalMoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* ApplyAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* CancelAction;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USEMenuButtonWidget* ContinueButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USEMenuButtonWidget* LoadGameButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USEMenuButtonWidget* NewGameButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USEMenuButtonWidget* SettingsButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USEMenuButtonWidget* ExitButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USELoadGameWidget* LoadGameWidget;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* HideMainMenu;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ShowMainMenu;
private:
	FWidgetAnimationDynamicEvent HideMainMenuDelegate;
	FWidgetAnimationDynamicEvent ShowMainMenuDelegate;

	UPROPERTY()
	USESubmenuWidget* CurrentSubmenu = nullptr;

	int32 CurrentIndex = -1;

	UFUNCTION()
	void OpenCurrentSubmenu();
	UFUNCTION()
	void CloseCurrentSubmenu();

	void MoveVertical(const FInputActionValue& Value);
	void Apply(const FInputActionValue& Value);
	void Cancel(const FInputActionValue& Value);

	UFUNCTION()
	void Continue();

	UFUNCTION()
	void LoadGame();

	UFUNCTION()
	void NewGame();

	UFUNCTION()
	void Settings();

	UFUNCTION()
	void Exit();
};
