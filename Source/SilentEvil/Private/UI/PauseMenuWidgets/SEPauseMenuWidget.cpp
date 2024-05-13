// Silent Evil Game, All Rights Reserved.

#include "UI/PauseMenuWidgets/SEPauseMenuWidget.h"

#include "UI/MainMenuWidgets/SEMenuButtonWidget.h"
#include "UI/MainMenuWidgets/SELoadGameWidget.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Kismet/GameplayStatics.h"
#include "Player/SEPlayerController.h"
#include "SEGameModeBase.h"

#include "Animation/WidgetAnimation.h"

DEFINE_LOG_CATEGORY_STATIC(LogSEPauseMenu, All, All);

void USEPauseMenuWidget::SetSelected(USEBaseButtonWidget* InButton)
{
	if (!Buttons.Contains(InButton))
	{
		return;
	}

	if (Buttons.IsValidIndex(CurrentIndex))
	{
		Buttons[CurrentIndex]->SetState(ESEButtonState::Normal);
	}

	CurrentIndex = Buttons.Find(Cast<USEMenuButtonWidget>(InButton));
	Buttons[CurrentIndex]->SetState(ESEButtonState::Hovered);
}

void USEPauseMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetupInput();

	Buttons.Add(ContinueButton);
	ContinueButton->OnClicked.AddDynamic(this, &USEPauseMenuWidget::Continue);
	ContinueButton->MenuWidget = this;

	Buttons.Add(LoadGameButton);
	LoadGameButton->OnClicked.AddDynamic(this, &USEPauseMenuWidget::LoadGame);
	LoadGameButton->MenuWidget = this;

	Buttons.Add(SettingsButton);
	SettingsButton->OnClicked.AddDynamic(this, &USEPauseMenuWidget::Settings);
	SettingsButton->MenuWidget = this;

	Buttons.Add(ExitButton);
	ExitButton->OnClicked.AddDynamic(this, &USEPauseMenuWidget::Exit);
	ExitButton->MenuWidget = this;

	ShowButtonsDelegate.BindDynamic(this, &USEPauseMenuWidget::CloseCurrentSubmenu);
	BindToAnimationFinished(ShowButtons, ShowButtonsDelegate);
}

void USEPauseMenuWidget::Continue()
{
	Close();
}

void USEPauseMenuWidget::LoadGame()
{
	PlayAnimationForward(HideButtons);

	LoadGameWidget->Open();
	CurrentSubmenu = LoadGameWidget;
}

void USEPauseMenuWidget::Settings()
{
	UE_LOG(LogSEPauseMenu, Display, TEXT("Settings"));
}

void USEPauseMenuWidget::Exit()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("MainMenu")));
}

void USEPauseMenuWidget::CloseCurrentSubmenu()
{
	CurrentSubmenu->Close();
	CurrentSubmenu = nullptr;
}

void USEPauseMenuWidget::MoveVertical(const FInputActionValue& Value)
{
	if (CurrentTargetWidget != nullptr)
	{
		CurrentTargetWidget->MoveVertical((int32)Value.Get<float>());
		return;
	}

	if (CurrentSubmenu != nullptr)
	{
		CurrentSubmenu->MoveVertical((int32)Value.Get<float>());
		return;
	}

	int32 Offset = -(int32)Value.Get<float>();

	if (Buttons.IsValidIndex(CurrentIndex))
	{
		Buttons[CurrentIndex]->SetState(ESEButtonState::Normal);
	}

	CurrentIndex = FMath::Clamp(CurrentIndex + Offset, 0, Buttons.Num() - 1);
	Buttons[CurrentIndex]->SetState(ESEButtonState::Hovered);
}

void USEPauseMenuWidget::Apply(const FInputActionValue& Value)
{
	if (CurrentTargetWidget != nullptr)
	{
		CurrentTargetWidget->Apply();
		return;
	}

	if (CurrentSubmenu != nullptr)
	{
		CurrentSubmenu->Apply();
		return;
	}

	if (Buttons.IsValidIndex(CurrentIndex))
	{
		Buttons[CurrentIndex]->Click();
	}
}

void USEPauseMenuWidget::Cancel(const FInputActionValue& Value)
{
	if (CurrentTargetWidget != nullptr)
	{
		if (CurrentTargetWidget->Cancel())
		{
			CurrentTargetWidget->Close();
			CurrentTargetWidget = nullptr;
		}
		return;
	}

	if (CurrentSubmenu != nullptr)
	{
		if (CurrentSubmenu->Cancel())
		{
			PlayAnimationForward(ShowButtons);
		}

		return;
	}

	Close();
}

void USEPauseMenuWidget::SetupInput()
{
	InitializeInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent == nullptr)
	{
		return;
	}

	EnhancedInputComponent->BindAction(VerticalMoveAction, ETriggerEvent::Triggered, this, &USEPauseMenuWidget::MoveVertical);
	EnhancedInputComponent->BindAction(ApplyAction, ETriggerEvent::Completed, this, &USEPauseMenuWidget::Apply);
	EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Completed, this, &USEPauseMenuWidget::Cancel);
}

void USEPauseMenuWidget::Close()
{
	ASEPlayerController* CharacterController = Cast<ASEPlayerController>(GetOwningPlayer());
	if (CharacterController != nullptr)
	{
		CharacterController->ResumeGame();
	}
}
