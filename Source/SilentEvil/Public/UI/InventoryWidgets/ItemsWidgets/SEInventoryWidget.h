// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/InventoryWidgets/SEBaseWidget.h"
#include "SEInventoryWidget.generated.h"

class UCanvasPanel;
class UUniformGridPanel;
class UGridPanel;
class UTextBlock;

class USEInventorySlotWidget;
class USEInventoryItemWidget;
class USEInventoryCursorWidget;
class USEDropDownMenuWidget;
class USEInspectWidget;
class USEInventoryComponent;

UCLASS()
class SILENTEVIL_API USEInventoryWidget : public USEBaseWidget
{
	GENERATED_BODY()


public:
	virtual bool Cancel() override;
	virtual void MoveItem() override;
	virtual void MoveVertical(float Value) override;
	virtual void MoveHorizontal(float Value) override;
	virtual void FastAccessItem(float Value) override;
	virtual void Apply() override;

	void UpdateInventoryData(bool OpenForInteraction = false);

	void RemoveItem(USEInventoryItemWidget* ItemWidget);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<USEInventorySlotWidget> InventorySlotWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<USEInventoryItemWidget> InventoryItemWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* SlotsGridPanel;

	UPROPERTY(meta = (BindWidget))
	UGridPanel* ItemsPanel;

	UPROPERTY(meta = (BindWidget))
	USEInventoryCursorWidget* InventoryCursor;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemDescription;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentHealth;

	UPROPERTY(meta = (BindWidget))
	USEDropDownMenuWidget* DropDownMenu;

	UPROPERTY(meta = (BindWidget))
	USEInspectWidget* InspectPanel;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* FirstAccessItemsPanel;

	virtual void NativeOnInitialized() override;

private:
	bool ForInteraction = false;

	bool DropDownMenuIsOpen = false;
	bool ItemIsMoving = false;
	bool ListenForBind = false;
	bool ItemInspect = false;

	UPROPERTY()
	USEInventoryItemWidget* CurrentItemWidget = nullptr;

	UPROPERTY()
	TMap<FIntPoint, USEInventorySlotWidget*> InventorySlots;

	UPROPERTY()
	TArray<USEInventoryItemWidget*> InventoryItems;

	UPROPERTY()
	TArray<USEInventoryItemWidget*> FastAccessItems;

	USEInventoryComponent* GetCharacterInventory();

	void MoveItemTo(const FIntPoint& Offset);
	void SetCurrentSelected();
	void SetCurrentUnSelected();

	void SetFastAccess(int32 Index);
	void StopFastAccess();

	void StopMoveItem();
	void CancelMoveItem();

	void OpenDropDownMenu();
	void CloseDropDownMenu();

	void StartInspectItem();
	void StopInspectItem();

	UFUNCTION()
	void StartMoveItem();
	UFUNCTION()
	void ExploreItem();
	UFUNCTION()
	void UseItem();
	UFUNCTION()
	void EquipWeaponItem();
	UFUNCTION()
	void FastAccessWeaponItem();
	UFUNCTION()
	void DropItem();

	UFUNCTION()
	void OnItemCrafted(USEItemData* NewItemData);

	void CreateSlots();
	void UpdateSlots();

	void HideFastAccessItems();
	void SetupFastAccessItems();
	void UpdateFastAccessItems();

	void UpdateItems();

	void HideHealth();
	void UpdateHealth();
};
