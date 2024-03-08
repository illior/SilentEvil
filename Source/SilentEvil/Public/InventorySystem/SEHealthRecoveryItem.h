// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/SEBaseItem.h"
#include "InventorySystem/Interfaces/SEUsableItemInterface.h"
#include "SEHealthRecoveryItem.generated.h"

/**
 * 
 */
UCLASS()
class SILENTEVIL_API USEHealthRecoveryItem : public USEBaseItem, public ISEUsableItemInterface
{
	GENERATED_BODY()


public:
	virtual bool CanUse(APawn* Pawn) override;
	virtual void Use(APawn* Pawn) override;

	float GetHealAmount() { return HealAmount; };

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealthRecovery", meta = (ClampMin = "1"))
	float HealAmount = 100.0f;
};
