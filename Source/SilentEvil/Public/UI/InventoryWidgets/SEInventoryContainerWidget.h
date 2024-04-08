// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SEInventoryContainerWidget.generated.h"

class UInputAction;
struct FInputActionValue;

class USEBaseWidget;
class USEMapWidget;
class USEInventoryWidget;
class USERecordsWidget;
class UHorizontalBox;
class UOverlay;
class UTextBlock;

class ASEInteractableTarget;

UCLASS()
class SILENTEVIL_API USEInventoryContainerWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	void OpenItems(bool ForInteraction = false, bool ShowItems = true);
	void OpenRecords();
	void OpenMap();

	void HideItems();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveVerticalAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveHorizontalAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AdditiveHorizontalAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveItemAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CancelAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ApplyAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FastAccessAction;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentWidgetName;

	UPROPERTY(meta = (BindWidget))
	USEMapWidget* MapWidget;

	UPROPERTY(meta = (BindWidget))
	USEInventoryWidget* InventoryWidget;

	UPROPERTY(meta = (BindWidget))
	USERecordsWidget* RecordsWidget;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* TopMenu;

	UPROPERTY(meta = (BindWidget))
	UOverlay* BackgroundBlur;

protected:
	bool CanChangeWidget = true;
	int32 CurrentIndex;
	UPROPERTY()
	TArray<USEBaseWidget*> Widgets;

	virtual void NativeOnInitialized() override;

private:
	ASEInteractableTarget* TargetObject = nullptr;

	void SetupInput();

	void AdditiveHorizontal(const FInputActionValue& Value);

	void Cancel();
	void MoveItem();
	void MoveVertical(const FInputActionValue& Value);
	void MoveHorizontal(const FInputActionValue& Value);
	void FastAccessItem(const FInputActionValue& Value);
	void Apply();
};
