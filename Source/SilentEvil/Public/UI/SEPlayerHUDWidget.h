// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SEPlayerHUDWidget.generated.h"

class UTextBlock;

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

	UFUNCTION(BlueprintCallable, Category = "UI")
	USEWeaponData* GetFastAccessWeapon(int32 Index);

	void SetHelpText(FText InText, float InTime);

protected:
	FTimerHandle HelpTextTimerHandle;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HelpText;

private:
	UFUNCTION()
	void HideText();

	USEInventoryComponent* GetCharacterInventory();
};
