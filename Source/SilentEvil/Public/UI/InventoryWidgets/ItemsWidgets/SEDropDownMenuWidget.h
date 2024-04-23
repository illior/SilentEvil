// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/InventoryWidgets/ItemsWidgets/SEDropDownButtonWidget.h"
#include "SEDropDownMenuWidget.generated.h"

class USEItemData;

UCLASS()
class SILENTEVIL_API USEDropDownMenuWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	void Open(USEItemData* ItemData, ESEInventoryOpenState InventoryState);
	void Close();

	bool IsActive();

	void MoveSelectedUp();
	void MoveSelectedDown();
	void ActivateSelected();

	FOnDropDownButtonClickedSignature& GetExploreButtonSignature();
	FOnDropDownButtonClickedSignature& GetUseButtonSignature();
	FOnDropDownButtonClickedSignature& GetEquipButtonSignature();
	FOnDropDownButtonClickedSignature& GetFastAccessButtonSignature();
	FOnDropDownButtonClickedSignature& GetStoreButtonSignature();
	FOnDropDownButtonClickedSignature& GetCombineButtonSignature();
	FOnDropDownButtonClickedSignature& GetDropButtonSignature();

protected:
	UPROPERTY(meta = (BindWidget))
	USEDropDownButtonWidget* ExploreButton;

	UPROPERTY(meta = (BindWidget))
	USEDropDownButtonWidget* UseButton;

	UPROPERTY(meta = (BindWidget))
	USEDropDownButtonWidget* EquipButton;

	UPROPERTY(meta = (BindWidget))
	USEDropDownButtonWidget* FastAccessButton;

	UPROPERTY(meta = (BindWidget))
	USEDropDownButtonWidget* StoreButton;

	UPROPERTY(meta = (BindWidget))
	USEDropDownButtonWidget* CombineButton;

	UPROPERTY(meta = (BindWidget))
	USEDropDownButtonWidget* DropButton;

	virtual void NativeOnInitialized() override;

private:
	int32 CurrentIndex = 0;

	UPROPERTY()
	TArray<USEDropDownButtonWidget*> Buttons;

	void ShowBase(USEItemData* ItemData);
	void ShowInteraction(USEItemData* ItemData);
	void ShowStorage(USEItemData* ItemData);
};
