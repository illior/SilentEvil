// Silent Evil Game, All Rights Reserved.

#include "UI/SEPlayerHUDWidget.h"

#include "InventorySystem/SEWeaponData.h"
#include "Components/SEInventoryComponent.h"

USEWeaponData* USEPlayerHUDWidget::GetCurrentWeapon()
{
	USEInventoryComponent* CharacterInventory = GetCharacterInventory();
	if (CharacterInventory == nullptr)
	{
		return nullptr;
	}

	return CharacterInventory->GetEquipWeapon();
}

int32 USEPlayerHUDWidget::GetCurrentWeaponAmmo()
{
	USEInventoryComponent* CharacterInventory = GetCharacterInventory();
	if (CharacterInventory == nullptr)
	{
		return 0;
	}

	USEWeaponData* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon == nullptr)
	{
		return 0;
	}

	return CharacterInventory->GetCountAmmoType(GetCurrentWeapon()->GetAmmoType());
}

USEInventoryComponent* USEPlayerHUDWidget::GetCharacterInventory()
{
	const APawn* Character = GetOwningPlayerPawn();
	if (Character == nullptr)
	{
		return nullptr;
	}

	USEInventoryComponent* CharacterInventory = Character->GetComponentByClass<USEInventoryComponent>();

	return CharacterInventory;
}
