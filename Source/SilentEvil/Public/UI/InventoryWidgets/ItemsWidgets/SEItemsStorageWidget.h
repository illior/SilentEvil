// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SEItemsStorageWidget.generated.h"

class USEItemData;
class USEStorageButtonWidget;
class UScrollBox;

UCLASS()
class SILENTEVIL_API USEItemsStorageWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void Hide();
	void Show();

	void AddItem(USEItemData* ItemData);
	void RemoveCurrentItem();

	bool SetFocus();
	void ClearFocus();

	void MoveUp();
	void MoveDown();

	void Apply();

	bool IsEmpty();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<USEStorageButtonWidget> ButtonClass;

	UPROPERTY()
	TArray<USEStorageButtonWidget*> StorageItems;
	int32 CurrentIndex;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* Scroll;

	virtual void NativeOnInitialized() override;

private:

};
