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

	virtual void Interact(APawn* Pawn);

	virtual void StartCanInteract(APawn* Pawn);
	virtual void StopCanInteract(APawn* Pawn);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void SetEnabled(bool NewValue);

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

//UPROPERTY(BlueprintAssignable, Category = "Game")
//FActorEndPlaySignature OnEndPlay;
//DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FActorDestroyedSignature, AActor, OnDestroyed, AActor*, DestroyedActor );