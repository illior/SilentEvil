// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SEItemData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDataChangedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDropSignature);

class USEBaseItem;
class ASEItemVisual;

UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class SILENTEVIL_API USEItemData : public UObject
{
	GENERATED_BODY()


	friend class USEInventoryComponent;
	friend class ASEPlayerState;
public:
	FOnDataChangedSignature OnDataChanged;
	FOnDropSignature OnDrop;

	UFUNCTION(BlueprintCallable, Category = "ItemData")
	USEBaseItem* GetItem() const;
	UFUNCTION(BlueprintCallable, Category = "ItemData")
	int32 GetCount() const;
	UFUNCTION(BlueprintCallable, Category = "ItemData")
	FIntPoint GetPosition() const;
	UFUNCTION(BlueprintCallable, Category = "ItemData")
	bool ContainsPoint(FIntPoint Point) const;

	UFUNCTION(BlueprintCallable, Category = "ItemData")
	FText GetItemName() const;
	UFUNCTION(BlueprintCallable, Category = "ItemData")
	FText GetItemDescription() const;
	UFUNCTION(BlueprintCallable, Category = "ItemData")
	bool GetItemCanStack() const;
	UFUNCTION(BlueprintCallable, Category = "ItemData")
	int32 GetItemMaxStackAmount() const;
	UFUNCTION(BlueprintCallable, Category = "ItemData")
	int32 GetItemSize() const;
	UFUNCTION(BlueprintCallable, Category = "ItemData")
	UTexture2D* GetItemIcon() const;
	UFUNCTION(BlueprintCallable, Category = "ItemData")
	ASEItemVisual* GetItemVisual() const;

	UFUNCTION(BlueprintCallable, Category = "ItemData")
	virtual bool CanDrop() const;

	UFUNCTION(BlueprintCallable, Category = "ItemData")
	virtual bool IsWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "ItemData")
	bool IsFull() const;

	virtual FString ToString() const;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemData")
	USEBaseItem* Item = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemData")
	int32 Count = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemData")
	FIntPoint Position = FIntPoint(0, 0);

	void SetPosition(const int& X, const int& Y);
	void SetCount(int32 NewCount);
	void Drop();
};
