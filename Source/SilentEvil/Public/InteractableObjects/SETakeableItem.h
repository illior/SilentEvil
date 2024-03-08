// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObjects/SEInteractableObject.h"
#include "SETakeableItem.generated.h"

class UStaticMeshComponent;
class USEBaseItem;

UCLASS()
class SILENTEVIL_API ASETakeableItem : public ASEInteractableObject
{
	GENERATED_BODY()

public:
	ASETakeableItem();

	virtual void Interact(APawn* Pawn) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	USEBaseItem* Item = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction", meta = (ClampMin = "1"))
	int32 ItemCount = 1;

	virtual void Disable() override;
};
