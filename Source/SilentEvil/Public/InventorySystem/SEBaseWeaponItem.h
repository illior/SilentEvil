// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/SEBaseItem.h"
#include "SECoreTypes.h"
#include "SEBaseWeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class SILENTEVIL_API USEBaseWeaponItem : public USEBaseItem
{
	GENERATED_BODY()


public:
	float GetDamage() const { return Damage; };
	int32 GetMaxAmmoInClip() const { return MaxAmmoInClip; }
	EAmmoType GetAmmoType() const { return AmmoType; };

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (ClampMin = "1"))
	float Damage = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (ClampMin = "1"))
	int32 MaxAmmoInClip = 7;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	EAmmoType AmmoType = EAmmoType::Pistol;
};
