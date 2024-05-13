// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SEInteractWidget.generated.h"

class USizeBox;

UCLASS()
class SILENTEVIL_API USEInteractWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void SetVisibleKey(bool NewValue);
	bool GetVisibleKey();

protected:
	UPROPERTY(meta = (BindWidget))
	USizeBox* KeyWidget;
};
