// Silent Evil Game, All Rights Reserved.

#include "InventorySystem/SEWeaponData.h"

#include "InventorySystem/SEBaseWeaponItem.h"

int32 USEWeaponData::GetMaxAmmoInClip() const
{
	USEBaseWeaponItem* WeaponItem = Cast<USEBaseWeaponItem>(Item);

	return WeaponItem == nullptr ? 1 : WeaponItem->GetMaxAmmoInClip();
}

int32 USEWeaponData::GetCurrentAmmoInClip() const
{
	return CurrentAmmoInClip;
}

int32 USEWeaponData::GetFastAccessIndex() const
{
	return FastAccessIndex;
}

bool USEWeaponData::GetIsEquip() const
{
	return IsEquip;
}

EAmmoType USEWeaponData::GetAmmoType() const
{
	USEBaseWeaponItem* WeaponItem = Cast<USEBaseWeaponItem>(Item);

	return WeaponItem == nullptr ? EAmmoType::Pistol : WeaponItem->GetAmmoType();
}

bool USEWeaponData::IsFullAmmo() const
{
	USEBaseWeaponItem* WeaponItem = Cast<USEBaseWeaponItem>(Item);

	return WeaponItem == nullptr ? false : CurrentAmmoInClip == WeaponItem->GetMaxAmmoInClip();
}

bool USEWeaponData::CanDrop() const
{
	return false;
}

void USEWeaponData::MakeShot()
{
	if (Item == nullptr || CurrentAmmoInClip == 0)
	{
		return;
	}

	// Shot logic
	CurrentAmmoInClip--;
	
	OnDataChanged.Broadcast();
}

void USEWeaponData::SetFastAccessIndex(int32 Index)
{
	FastAccessIndex = Index;

	OnDataChanged.Broadcast();
}

void USEWeaponData::SetEquip(bool NewValue)
{
	IsEquip = NewValue;

	OnDataChanged.Broadcast();
}

void USEWeaponData::SetCurrentAmmoInClip(int32 NewCount)
{
	USEBaseWeaponItem* WeaponItem = Cast<USEBaseWeaponItem>(Item);

	if (WeaponItem != nullptr)
	{
		CurrentAmmoInClip = FMath::Clamp(NewCount, 0, WeaponItem->GetMaxAmmoInClip());
	}
	else
	{
		CurrentAmmoInClip = 0;
	}

	OnDataChanged.Broadcast();
}
