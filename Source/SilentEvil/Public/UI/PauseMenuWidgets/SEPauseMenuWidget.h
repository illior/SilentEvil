// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SEPauseMenuWidget.generated.h"

class UInputAction;
struct FInputActionValue;

UCLASS()
class SILENTEVIL_API USEPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CancelAction;

	virtual void NativeOnInitialized() override;

private:
	void SetupInput();

	void Close();
};
