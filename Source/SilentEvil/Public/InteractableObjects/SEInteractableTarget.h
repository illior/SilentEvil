// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObjects/SEInteractableObject.h"
#include "SEInteractableTarget.generated.h"

class UCameraComponent;
class USEBaseItem;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FCompleteSignature, ASEInteractableTarget, OnComplete, ASEInteractableTarget*, ActivatedObject, APawn*, ActivatedBy);

UCLASS()
class SILENTEVIL_API ASEInteractableTarget : public ASEInteractableObject
{
	GENERATED_BODY()


public:
	ASEInteractableTarget();

	virtual void Interact(APawn* Pawn) override;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void Complete();

	virtual void MoveVertical(const float& Value);
	virtual void MoveHorizontal(const float& Value);
	virtual void Apply();

	virtual void Close();

	virtual bool TryUseItem(USEBaseItem* InItem);

	bool GetRemoveItem() const;

	FVector GetCameraLocation() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool WithInventory = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (EditCondition = "WithInventory"))
	TArray<USEBaseItem*> InteractableItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (EditCondition = "WithInventory"))
	bool RemoveItem = true;

	UPROPERTY(BlueprintReadWrite)
	APawn* CurrentPawn = nullptr;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "MoveVertical"))
	void ReceiveMoveVertical(const float& Value);
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "MoveHorizontal"))
	void ReceiveMoveHorizontal(const float& Value);
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "Apply"))
	void ReceiveApply();
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "Close"))
	void ReceiveClose();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "ItemUse"))
	void ReceiveItemUse(USEBaseItem* InItem);

public:
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FCompleteSignature OnComplete;
};
