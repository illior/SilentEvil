// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SEInteractableObject.generated.h"

class UCapsuleComponent;
class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FInteractSignature, ASEInteractableObject, OnInteract, ASEInteractableObject*, ActivatedObject, APawn*, ActivatedBy);

UCLASS()
class SILENTEVIL_API ASEInteractableObject : public AActor
{
	GENERATED_BODY()
	

public:
	ASEInteractableObject();

	UFUNCTION(BlueprintCallable, Category = "Interaction", meta = (DisplayName = "CallInteract"))
	virtual void Interact(APawn* Pawn);

	UFUNCTION(BlueprintCallable, Category = "Interaction", meta = (DisplayName = "CallStartCanInteract"))
	virtual void StartCanInteract(APawn* Pawn);
	UFUNCTION(BlueprintCallable, Category = "Interaction", meta = (DisplayName = "CallStopCanInteract"))
	virtual void StopCanInteract(APawn* Pawn);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void SetEnabled(bool NewValue);
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual bool GetEnabled() const { return IsEnabled; }

	UFUNCTION(BlueprintCallable, Category = "Actor")
	virtual FVector GetTargetLocation(AActor* RequestedBy = nullptr) const override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCapsuleComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool DisableAfterInteract = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool IsEnabled = true;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	float DistanceToInteract = 200.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	APawn* CurrentPawn = nullptr;

	virtual void Disable();
	virtual void Enable();

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "StartCanInteract"))
	void ReceiveStartCanInteract(APawn* Pawn);
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "StopCanInteract"))
	void ReceiveStopCanInteract(APawn* Pawn);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "Interact"))
	void ReceiveInteract(APawn* Pawn);
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FInteractSignature OnInteract;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "Disable"))
	void ReceiveDisable();
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "Enable"))
	void ReceiveEnable();
};
