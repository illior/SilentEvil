// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/SEBaseItem.h"
#include "SECoreTypes.h"
#include "SEAmmoItem.generated.h"

/**
 * 
 */
UCLASS()
class SILENTEVIL_API USEAmmoItem : public USEBaseItem
{
	GENERATED_BODY()


public:
	EAmmoType GetAmmoType() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	EAmmoType AmmoType = EAmmoType::Pistol;
};
