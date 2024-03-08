// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SEGameHUD.generated.h"

class USEInventoryContainerWidget;

UCLASS()
class SILENTEVIL_API ASEGameHUD : public AHUD
{
	GENERATED_BODY()


public:
	void OpenTargetInventory(bool ShowItems);

	void OpenInventory();
	void OpenMenu();
	
	void Close();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<USEInventoryContainerWidget> InventoryWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY()
	USEInventoryContainerWidget* InventoryWidget;

	UPROPERTY()
	UUserWidget* PauseMenuWidget;

	UPROPERTY()
	UUserWidget* PlayerHUDWidget;

	virtual void BeginPlay() override;

private:

};
