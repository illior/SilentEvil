// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObjects/SEInteractableTarget.h"
#include "SEStorageBox.generated.h"

UCLASS()
class SILENTEVIL_API ASEStorageBox : public ASEInteractableTarget
{
	GENERATED_BODY()


public:
	ASEStorageBox();

	virtual void Interact(APawn* Pawn) override;

	virtual void MoveVertical(const float& Value);
	virtual void MoveHorizontal(const float& Value);
	virtual void Apply();

	virtual void Close();

protected:

};
