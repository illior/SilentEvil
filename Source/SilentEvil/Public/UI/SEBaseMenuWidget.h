// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SEBaseMenuWidget.generated.h"

class USEBaseButtonWidget;

UCLASS()
class SILENTEVIL_API USEBaseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void SetSelected(USEBaseButtonWidget* InButton);
};
