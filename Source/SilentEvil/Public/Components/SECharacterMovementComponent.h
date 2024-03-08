// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SECharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SILENTEVIL_API USECharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()


public:
	virtual float GetMaxSpeed() const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement: Sprint", meta = (ClampMin = "0.0", ForceUnits = "cm/s"))
	float MaxSprintSpeed = 600.0f;
};
