// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SECoreTypes.h"
#include "SEInventoryItemWidget.generated.h"

class USEItemData;

class UImage;
class UTextBlock;
class USEInventoryWidget;

UCLASS()
class SILENTEVIL_API USEInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY()
	USEInventoryWidget* ParentWidget = nullptr;

	void SetItemData(USEItemData* NewItemData);
	USEItemData* GetItemData();

	void SetViewState(EViewState NewState);
	EViewState GetViewState();

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage;

	UPROPERTY(meta = (BindWidget))
	UImage* SelectedImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* FastAccessText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EquipText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Appearance")
	FLinearColor FullCountText = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Appearance")
	FLinearColor NotFullCountText = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Appearance")
	FLinearColor NormalColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Appearance")
	FLinearColor SelectedColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.5f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Appearance")
	FLinearColor ActiveColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FSlateBrush EmptyItem = FSlateBrush();

	float InitialWidth = 100.0f;

private:
	EViewState CurrentViewState = EViewState::Normal;

	UPROPERTY()
	USEItemData* ItemData;

	UFUNCTION()
	void OnDataChanged();
	UFUNCTION()
	void OnDropItem();

	void Place();
	void SetEmptyItem();

	void UpdateData();
	void UpdatePosition();
};
