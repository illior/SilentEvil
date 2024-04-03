// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SECharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class USEInventoryComponent;
class USEHealthComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class ASEInteractableObject;

UCLASS()
class SILENTEVIL_API ASECharacter : public ACharacter
{
	GENERATED_BODY()


public:
	ASECharacter(const FObjectInitializer& ObjInit);

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetShouldSprint() const;
	float GetDistanceForInteraction() const;

	UCameraComponent* GetCameraComponent() const;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* FirstPersonMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USEInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USEHealthComponent* HealthComponent;

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

	virtual void BeginPlay() override;

private:
	bool ShouldSprint = false;
	bool IsAim = false;

	UPROPERTY()
	ASEInteractableObject* CurrentInteractableObject = nullptr;

	UPROPERTY()
	UCameraShakeBase* CameraShakeInstance = nullptr;

	void SearchInteractableObject();

	void StartMove(const FInputActionValue& Value);
	void StopMove(const FInputActionValue& Value);

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void Sprint(const FInputActionValue& Value);
	void StartSprint();
	void StopSprint();

	void Reload(const FInputActionValue& Value);

	void StartFire(const FInputActionValue& Value);
	void StopFire(const FInputActionValue& Value);

	void StartAim(const FInputActionValue& Value);
	void StopAim(const FInputActionValue& Value);

	void Interact();
};
