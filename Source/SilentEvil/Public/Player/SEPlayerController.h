// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SEPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

class ASEInteractableTarget;

UCLASS()
class SILENTEVIL_API ASEPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "Pause")
	void ResumeGame();

	void SetInteractTarget(ASEInteractableTarget* Target, bool ShowItems = false);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* OpenUIMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* LocomotionMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InventoryMenuMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* PauseMenuMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* OpenMapAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* OpenInventoryAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* OpenRecordsAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* OpenMenuAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraBlend")
	float BlendTime = 0.2f;

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:
	FTimerHandle InvokeTimerHandle;

	void SetPause();

	UFUNCTION()
	void OpenTargetMenu(bool ShowItems);

	void OpenMap();
	void OpenInventory();
	void OpenRecords();
	void OpenMenu();

	void SetGameMappingContext();
	void SetMappingContext(UInputMappingContext* NewMappingContext);
};
