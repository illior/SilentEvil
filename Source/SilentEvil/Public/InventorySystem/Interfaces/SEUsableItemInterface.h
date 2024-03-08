// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SEUsableItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class USEUsableItemInterface : public UInterface
{
	GENERATED_BODY()
};

class SILENTEVIL_API ISEUsableItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	virtual bool CanUse(APawn* Pawn);
	virtual void Use(APawn* Pawn);

};
