// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SaveSystem/SESavableObject.h"
#include "SECharacter.generated.h"

class USpringArmComponent;
class USECameraComponent;
class USkeletalMeshComponent;
class USEInventoryComponent;
class USEHealthComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class ASEInteractableObject;

UCLASS()
class SILENTEVIL_API ASECharacter : public ACharacter, public ISESavableObject
{
	GENERATED_BODY()


public:
	ASECharacter(const FObjectInitializer& ObjInit);

	virtual FSESaveDataRecord GetSaveDataRecord_Implementation();
	virtual void LoadFromSaveDataRecord_Implementation(FSESaveDataRecord InRecord);

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetShouldSprint() const;
	float GetDistanceForInteraction() const;

	USECameraComponent* GetCameraComponent() const;

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USECameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* FirstPersonMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USEInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USEHealthComponent* HealthComponent;

	UPROPERTY(SaveGame)
	TArray<FSESaveDataComponent> ComponentsData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* ReloadAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* AimAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* FireAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* FastAccessAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* CrouchAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	TSubclassOf<UCameraShakeBase> CameraWalkShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	TSubclassOf<UCameraShakeBase> CameraSprintShake;

	float CurrentDustanceToInteractionObject = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	float CloseDistanceInteraction = 150.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	float DistanceForSearchInteraction = 600.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	float CapsuleRadiusForSearchInteraction = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float StandCameraHeight = 60.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float CrouchCameraHeight = 40.0f;

	virtual void BeginPlay() override;

private:
	bool ShouldSprint = false;
	bool IsAim = false;

	UPROPERTY()
	ASEInteractableObject* CurrentInteractableObject = nullptr;

	UPROPERTY()
	UCameraShakeBase* CameraShakeInstance = nullptr;

	UPROPERTY(SaveGame)
	FRotator ControlRotation;

	void SearchInteractableObject();

	void InputStartMove(const FInputActionValue& Value);
	void InputStopMove(const FInputActionValue& Value);

	void InputMove(const FInputActionValue& Value);
	void InputLook(const FInputActionValue& Value);

	void InputCrouch(const FInputActionValue& Value);
	void StartCrouch();
	void StopCrouch();

	void InputSprint(const FInputActionValue& Value);
	void StartSprint();
	void StopSprint();

	void InputReload(const FInputActionValue& Value);

	void InputStartFire(const FInputActionValue& Value);
	void InputStopFire(const FInputActionValue& Value);

	void InputStartAim(const FInputActionValue& Value);
	void InputStopAim(const FInputActionValue& Value);

	void InputFastAccessItem(const FInputActionValue& Value);

	void InputInteract(const FInputActionValue& Value);
};
