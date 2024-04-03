// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SEBaseWidget.generated.h"

UCLASS()
class SILENTEVIL_API USEBaseWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual bool Cancel();
	virtual void MoveItem();
	virtual void MoveVertical(float Value);
	virtual void MoveHorizontal(float Value);
	virtual void FastAccessItem(float Value);
	virtual void Apply();

protected:

};
