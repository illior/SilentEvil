// Silent Evil Game, All Rights Reserved.

#include "InteractableObjects/SEInteractableObject.h"

#include "Player/SECharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSEInteractableObject, All, All);

ASEInteractableObject::ASEInteractableObject()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(SceneComponent);

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>("Collision");
	CollisionComponent->SetupAttachment(GetRootComponent());
	CollisionComponent->SetCollisionProfileName(FName("OnlyInteract"));

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget");
	WidgetComponent->SetupAttachment(GetRootComponent());
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetVisibility(false);

	WidgetComponent->SetDrawSize(FVector2D(50.0f, 50.0f));
	WidgetComponent->SetDrawAtDesiredSize(true);
	WidgetComponent->SetPivot(FVector2D(0.5f, 1.0f));
}

void ASEInteractableObject::Interact(APawn* Pawn)
{
	ReceiveInteract(Pawn);

	OnInteract.Broadcast(this, Pawn);

	StopCanInteract(Pawn);
	if (DisableAfterInteract)
	{
		Disable();
	}
}

void ASEInteractableObject::StartCanInteract(APawn* Pawn)
{
	WidgetComponent->SetVisibility(true);

	if (Pawn->IsA<ASECharacter>())
	{
		ASECharacter* Character = Cast<ASECharacter>(Pawn);

		DistanceToInteract = Character->GetDistanceForInteraction();
	}

	CurrentPawn = Pawn;

	ReceiveStartCanInteract(Pawn);
}

void ASEInteractableObject::StopCanInteract(APawn* Pawn)
{
	WidgetComponent->SetVisibility(false);

	CurrentPawn = nullptr;

	ReceiveStopCanInteract(Pawn);
}

void ASEInteractableObject::SetEnabled(bool NewValue)
{
	if (NewValue == IsEnabled)
	{
		return;
	}

	NewValue ? Enable() : Disable();
}

FVector ASEInteractableObject::GetTargetLocation(AActor* RequestedBy) const
{
	return CollisionComponent->GetComponentLocation();
}

void ASEInteractableObject::Disable()
{
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	IsEnabled = false;

	ReceiveDisable();
}

void ASEInteractableObject::Enable()
{
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	IsEnabled = true;

	ReceiveEnable();
}

void ASEInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsEnabled)
	{
		Disable();
	}
}
