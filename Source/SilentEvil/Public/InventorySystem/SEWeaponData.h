// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/SEItemData.h"
#include "SECoreTypes.h"
#include "SEWeaponData.generated.h"

UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class SILENTEVIL_API USEWeaponData : public USEItemData
{
	GENERATED_BODY()


	friend class USEInventoryComponent;
public:
	UFUNCTION(BlueprintCallable, Category = "ItemData")
	int32 GetMaxAmmoInClip() const;

	UFUNCTION(BlueprintCallable, Category = "ItemData")
	int32 GetCurrentAmmoInClip() const;
	UFUNCTION(BlueprintCallable, Category = "ItemData")
	int32 GetFastAccessIndex() const;
	UFUNCTION(BlueprintCallable, Category = "ItemData")
	bool GetIsEquip() const;
	UFUNCTION(BlueprintCallable, Category = "ItemData")
	EAmmoType GetAmmoType() const;
	UFUNCTION(BlueprintCallable, Category = "ItemData")
	bool IsFullAmmo() const;

	virtual bool CanDrop() const override;

	virtual void MakeShot();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponData", meta = (ClampMin = "0"))
	int32 CurrentAmmoInClip = 7;

	int32 FastAccessIndex = -1;

	bool IsEquip = false;

	virtual void SetFastAccessIndex(int32 Index);
	virtual void SetEquip(bool NewValue);

	virtual void SetCurrentAmmoInClip(int32 NewCount);
};
