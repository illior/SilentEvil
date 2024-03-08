// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SEBaseItem.generated.h"

class ASEItemVisual;

UCLASS(BlueprintType)
class SILENTEVIL_API USEBaseItem : public UDataAsset
{
	GENERATED_BODY()


public:
	FName GetName() const { return Name; }
	bool GetCanStack() const { return CanStack; }
	int32 GetMaxStackAmount() const { return MaxStackAmount; }
	int32 GetSize() const { return Size; }
	UTexture2D* GetIcon() const { return Icon; }
	ASEItemVisual* GetVisual(UWorld* World);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FName Name = "unique_name";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	bool CanStack = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (ClampMin = "1", ClampMax = "400", EditCondition = "CanStack"))
	int32 MaxStackAmount = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (ClampMin = "1", ClampMax = "2"))
	int32 Size = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TSubclassOf<ASEItemVisual> ItemVisualClass;

	UPROPERTY()
	ASEItemVisual* ItemVisual = nullptr;
};
