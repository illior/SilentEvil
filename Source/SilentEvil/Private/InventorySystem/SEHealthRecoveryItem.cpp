// Silent Evil Game, All Rights Reserved.

#include "InventorySystem/SEHealthRecoveryItem.h"

#include "Components/SEHealthComponent.h"

bool USEHealthRecoveryItem::CanUse(APawn* Pawn)
{
	if (Pawn == nullptr)
	{
		return false;
	}

	const USEHealthComponent* CharacterHealth = Pawn->FindComponentByClass<USEHealthComponent>();
	if (CharacterHealth == nullptr)
	{
		return false;
	}

	return !CharacterHealth->GetIsDead() && (CharacterHealth->GetMaxHealtht() > CharacterHealth->GetCurrentHealth());
}

void USEHealthRecoveryItem::Use(APawn* Pawn)
{
	if (Pawn == nullptr)
	{
		return;
	}

	USEHealthComponent* CharacterHealth = Pawn->FindComponentByClass<USEHealthComponent>();
	if (CharacterHealth == nullptr)
	{
		return;
	}

	CharacterHealth->GetHeal(HealAmount);
}
