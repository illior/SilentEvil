// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SEPlayerHUDWidget.generated.h"

class USEWeaponData;
class USEInventoryComponent;

UCLASS()
class SILENTEVIL_API USEPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	USEWeaponData* GetCurrentWeapon();

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetCurrentWeaponAmmo();

private:
	USEInventoryComponent* GetCharacterInventory();
};
