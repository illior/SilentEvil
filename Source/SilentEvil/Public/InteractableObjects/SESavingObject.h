// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObjects/SEInteractableTarget.h"
#include "SESavingObject.generated.h"

class USESubmenuWidget;

UCLASS()
class SILENTEVIL_API ASESavingObject : public ASEInteractableTarget
{
	GENERATED_BODY()
	

public:
	virtual void Interact(APawn* Pawn) override;

	USESubmenuWidget* GetWidget();
	void SetWidget(USESubmenuWidget* InWidget);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<USESubmenuWidget> WidgetClass;

	UPROPERTY()
	USESubmenuWidget* Widget;

	void FindOrCreateWidget();
};
