// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SEGameHUD.generated.h"

class USESubmenuWidget;
class USEInventoryContainerWidget;
class USEPauseMenuWidget;
class USEPlayerHUDWidget;

UCLASS()
class SILENTEVIL_API ASEGameHUD : public AHUD
{
	GENERATED_BODY()


public:
	void OpenTargetInventory(bool ShowItems);
	void OpenInventory();

	void OpenRecords();

	void OpenMap();

	void OpenTargetWiget(USESubmenuWidget* WidgetToFocus);

	void OpenMenu();
	
	void Close();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ShowHelpText(FText InText, float Time = 5.0f);

	USEInventoryContainerWidget* GetInventoryWidget();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<USEInventoryContainerWidget> InventoryWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<USEPauseMenuWidget> PauseMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY()
	USEInventoryContainerWidget* InventoryWidget;

	UPROPERTY()
	USEPauseMenuWidget* PauseMenuWidget;

	UPROPERTY()
	USEPlayerHUDWidget* PlayerHUDWidget;

	virtual void BeginPlay() override;

};
