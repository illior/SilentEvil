// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SEBaseWidget.h"
#include "SEMapWidget.generated.h"

UCLASS()
class SILENTEVIL_API USEMapWidget : public USEBaseWidget
{
	GENERATED_BODY()


public:
	virtual bool Cancel() override;
	virtual void MoveVertical(float Value) override;
	virtual void MoveHorizontal(float Value) override;
	virtual void Apply() override;
};
