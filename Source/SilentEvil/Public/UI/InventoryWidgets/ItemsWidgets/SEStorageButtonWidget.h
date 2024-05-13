// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SEBaseButtonWidget.h"
#include "SEStorageButtonWidget.generated.h"

class USEItemData;

UCLASS()
class SILENTEVIL_API USEStorageButtonWidget : public USEBaseButtonWidget
{
	GENERATED_BODY()
	

public:
	void SetItemData(USEItemData* InItemData);
	USEItemData* GetItemData();

protected:
	UPROPERTY()
	USEItemData* ItemData;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemCount;

private:
	UFUNCTION()
	void UpdateCount();
};
