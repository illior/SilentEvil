// Silent Evil Game, All Rights Reserved.

#include "UI/MainMenuWidgets/SEMainMenuWidget.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "UI/MainMenuWidgets/SELoadGameWidget.h"
#include "UI/MainMenuWidgets/SEMenuButtonWidget.h"

#include "Animation/WidgetAnimation.h"

void USEMainMenuWidget::SetSelected(USEBaseButtonWidget* InButton)
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

void USEMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetupInput();

	Buttons.Add(ContinueButton);
	ContinueButton->OnClicked.AddDynamic(this, &USEMainMenuWidget::Continue);
	ContinueButton->MenuWidget = this;

	Buttons.Add(LoadGameButton);
	LoadGameButton->OnClicked.AddDynamic(this, &USEMainMenuWidget::LoadGame);
	LoadGameButton->MenuWidget = this;

	Buttons.Add(NewGameButton);
	NewGameButton->OnClicked.AddDynamic(this, &USEMainMenuWidget::NewGame);
	NewGameButton->MenuWidget = this;

	Buttons.Add(SettingsButton);
	SettingsButton->OnClicked.AddDynamic(this, &USEMainMenuWidget::Settings);
	SettingsButton->MenuWidget = this;

	Buttons.Add(ExitButton);
	ExitButton->OnClicked.AddDynamic(this, &USEMainMenuWidget::Exit);
	ExitButton->MenuWidget = this;

	HideMainMenuDelegate.BindDynamic(this, &USEMainMenuWidget::OpenCurrentSubmenu);
	BindToAnimationFinished(HideMainMenu, HideMainMenuDelegate);

	ShowMainMenuDelegate.BindDynamic(this, &USEMainMenuWidget::CloseCurrentSubmenu);
	BindToAnimationFinished(ShowMainMenu, ShowMainMenuDelegate);
}

void USEMainMenuWidget::SetupInput()
{
	InitializeInputComponent();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningLocalPlayer());
	if (Subsystem == nullptr)
	{
		return;
	}

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(MainMenuMappingContext, 0);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent == nullptr)
	{
		return;
	}

	EnhancedInputComponent->BindAction(VerticalMoveAction, ETriggerEvent::Triggered, this, &USEMainMenuWidget::MoveVertical);
	EnhancedInputComponent->BindAction(ApplyAction, ETriggerEvent::Completed, this, &USEMainMenuWidget::Apply);
	EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Completed, this, &USEMainMenuWidget::Cancel);
}

bool USEMainMenuWidget::IsMenuAnimationPlaying()
{
	return IsAnimationPlaying(HideMainMenu) || IsAnimationPlaying(ShowMainMenu);
}

void USEMainMenuWidget::OpenCurrentSubmenu()
{
	
}

void USEMainMenuWidget::CloseCurrentSubmenu()
{
	CurrentSubmenu->Close();
	CurrentSubmenu = nullptr;
}

void USEMainMenuWidget::MoveVertical(const FInputActionValue& Value)
{
	if (IsMenuAnimationPlaying())
	{
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

void USEMainMenuWidget::Apply(const FInputActionValue& Value)
{
	if (IsMenuAnimationPlaying())
	{
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

void USEMainMenuWidget::Cancel(const FInputActionValue& Value)
{
	if (IsMenuAnimationPlaying())
	{
		return;
	}

	if (CurrentSubmenu != nullptr)
	{
		if (CurrentSubmenu->Cancel())
		{
			PlayAnimationForward(ShowMainMenu);
		}
		return;
	}
}

void USEMainMenuWidget::Continue()
{
	if (IsMenuAnimationPlaying())
	{
		return;
	}
}

void USEMainMenuWidget::LoadGame()
{
	if (IsMenuAnimationPlaying())
	{
		return;
	}
	
	PlayAnimationForward(HideMainMenu);

	CurrentSubmenu = LoadGameWidget;
	CurrentSubmenu->Open();
}

void USEMainMenuWidget::NewGame()
{
	if (IsMenuAnimationPlaying())
	{
		return;
	}
}

void USEMainMenuWidget::Settings()
{
	if (IsMenuAnimationPlaying())
	{
		return;
	}
}

void USEMainMenuWidget::Exit()
{
	if (IsMenuAnimationPlaying())
	{
		return;
	}
}
