// Silent Evil Game, All Rights Reserved.

#include "UI/SEPlayerHUDWidget.h"

#include "Engine/World.h"

#include "Components/TextBlock.h"

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

USEWeaponData* USEPlayerHUDWidget::GetFastAccessWeapon(int32 Index)
{
	USEInventoryComponent* CharacterInventory = GetCharacterInventory();
	if (CharacterInventory == nullptr)
	{
		return nullptr;
	}

	return CharacterInventory->GetFastAccessWeapon(Index);
}

void USEPlayerHUDWidget::SetHelpText(FText InText, float InTime)
{
	GetWorld()->GetTimerManager().ClearTimer(HelpTextTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(HelpTextTimerHandle, this, &USEPlayerHUDWidget::HideText, InTime, false);

	HelpText->SetText(InText);
	HelpText->SetVisibility(ESlateVisibility::Visible);
}

void USEPlayerHUDWidget::HideText()
{
	HelpText->SetVisibility(ESlateVisibility::Hidden);
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
