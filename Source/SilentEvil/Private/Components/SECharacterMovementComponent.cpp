// Silent Evil Game, All Rights Reserved.

#include "Components/SECharacterMovementComponent.h"

#include "Player/SECharacter.h"

float USECharacterMovementComponent::GetMaxSpeed() const
{
	ASECharacter* Character = Cast<ASECharacter>(GetOwner());
	if (Character == nullptr)
	{
		return Super::GetMaxSpeed();
	}

	if (Character->GetShouldSprint())
	{
		return MaxSprintSpeed;
	}

	return Super::GetMaxSpeed();
}
