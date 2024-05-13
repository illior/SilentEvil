// Silent Evil Game, All Rights Reserved.

#include "InteractableObjects/SEInteractableObject.h"

#include "Player/SECharacter.h"

#include "UI/WidgetComponents/SEInteractWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SECameraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSEInteractableObject, All, All);

ASEInteractableObject::ASEInteractableObject()
{
	PrimaryActorTick.bCanEverTick = true;

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
		ShowKey = true;
	}

	CurrentPawn = Pawn;

	ReceiveStartCanInteract(Pawn);
}

void ASEInteractableObject::StopCanInteract(APawn* Pawn)
{
	WidgetComponent->SetVisibility(false);

	if (Pawn->IsA<ASECharacter>())
	{
		ShowKey = false;
	}

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

FSESaveDataRecord ASEInteractableObject::GetSaveDataRecord_Implementation()
{
	FSESaveDataRecord Record = FSESaveDataRecord();

	Record.ActorClass = GetClass();
	Record.ActorName = GetName();
	Record.ActorTransform = GetTransform();

	FMemoryWriter Writer = FMemoryWriter(Record.BinaryData);
	FObjectAndNameAsStringProxyArchive Ar(Writer, false);
	Ar.ArIsSaveGame = true;

	Serialize(Ar);

	return Record;
}

void ASEInteractableObject::LoadFromSaveDataRecord_Implementation(FSESaveDataRecord InRecord)
{
	SetActorTransform(InRecord.ActorTransform);

	FMemoryReader Reader = FMemoryReader(InRecord.BinaryData);
	FObjectAndNameAsStringProxyArchive Ar(Reader, false);
	Ar.ArIsSaveGame = true;

	Serialize(Ar);
}

void ASEInteractableObject::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!ShowKey)
	{
		return;
	}

	USEInteractWidget* InteractWidget = Cast<USEInteractWidget>(WidgetComponent->GetUserWidgetObject());
	if (InteractWidget == nullptr)
	{
		return;
	}

	InteractWidget->SetVisibleKey(GetDistance() < DistanceToInteract);
}

float ASEInteractableObject::GetDistance() const
{
	ASECharacter* Character = Cast<ASECharacter>(CurrentPawn);
	if (Character != nullptr)
	{
		FVector CameraLocation = Character->GetCameraComponent()->GetComponentLocation();

		return FVector::Distance(CameraLocation, GetTargetLocation());
	}

	return 0.0f;
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
	
	USEInteractWidget* InteractWidget = Cast<USEInteractWidget>(WidgetComponent->GetUserWidgetObject());
	if (InteractWidget != nullptr)
	{
		InteractWidget->SetVisibleKey(false);
	}

	IsEnabled ? Enable() : Disable();
}
