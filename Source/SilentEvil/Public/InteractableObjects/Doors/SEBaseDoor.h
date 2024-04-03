// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObjects/SEInteractableObject.h"
#include "SEBaseDoor.generated.h"

class UTimelineComponent;
class UArrowComponent;

UCLASS()
class SILENTEVIL_API ASEBaseDoor : public ASEInteractableObject
{
	GENERATED_BODY()


public:
	ASEBaseDoor();

	virtual void StartCanInteract(APawn* Pawn) override;
	virtual void StopCanInteract(APawn* Pawn) override;

	virtual void Interact(APawn* Pawn) override;

	virtual FVector GetTargetLocation(AActor* RequestedBy = nullptr) const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* DoorFrameMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTimelineComponent* TimeLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool IsDoorLocked = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	UCurveFloat* CurveMovement = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	float OpenAngle = 80.0f;

	float TargetAngle = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool IsOpen = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool IsMove = false;

	virtual void Disable() override;
	virtual void Enable() override;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void Open(APawn* Pawn);
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void Close();

	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void TimeLineUpdate(float Alpha);
	UFUNCTION()
	void StopMoveDoor();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	float GetAngle(FVector PawnLocation);
};
