// Silent Evil Game, All Rights Reserved.

#include "UI/PauseMenuWidgets/SEPauseMenuWidget.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Player/SEPlayerController.h"
#include "SEGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogSEPauseMenu, All, All);

void USEPauseMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetupInput();
}

void USEPauseMenuWidget::SetupInput()
{
	ASEPlayerController* Controller = GetOwningPlayer<ASEPlayerController>();
	if (Controller == nullptr)
	{
		return;
	}

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(Controller->InputComponent);
	if (EnhancedInputComponent == nullptr)
	{
		return;
	}

	EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Started, this, &USEPauseMenuWidget::Close);
}

void USEPauseMenuWidget::Close()
{
	ASEPlayerController* CharacterController = Cast<ASEPlayerController>(GetOwningPlayer());
	if (CharacterController != nullptr)
	{
		CharacterController->ResumeGame();
	}
}
