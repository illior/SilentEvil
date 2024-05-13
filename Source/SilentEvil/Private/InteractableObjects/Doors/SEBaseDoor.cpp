// Silent Evil Game, All Rights Reserved.

#include "InteractableObjects/Doors/SEBaseDoor.h"

#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/ArrowComponent.h"

ASEBaseDoor::ASEBaseDoor()
{
	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>("DoorFrame");
	DoorFrameMesh->SetupAttachment(GetRootComponent());
	DoorFrameMesh->SetCollisionProfileName(FName("BlockAll"));

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>("Door");
	DoorMesh->SetupAttachment(GetRootComponent());
	DoorMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("Arrow");
	ArrowComponent->SetupAttachment(DoorFrameMesh);

	TimeLine = CreateDefaultSubobject<UTimelineComponent>("DoorTimeLine");

	WidgetComponent->SetupAttachment(DoorMesh);

	CollisionComponent->SetCollisionProfileName(FName("NoCollision"));
}

void ASEBaseDoor::Interact(APawn* Pawn)
{
	if (Pawn == nullptr || IsMove)
	{
		return;
	}
	
	if (!IsDoorLocked)
	{
		IsOpen ? Close() : Open(Pawn);
		Disable();
	}

	Super::Interact(Pawn);
}

FVector ASEBaseDoor::GetTargetLocation(AActor* RequestedBy) const
{
	return WidgetComponent->GetComponentLocation();
}

void ASEBaseDoor::Disable()
{
	IsEnabled = false;
	WidgetComponent->SetVisibility(false);

	ReceiveDisable();
}

void ASEBaseDoor::Enable()
{
	IsEnabled = true;

	ReceiveEnable();
}

void ASEBaseDoor::Open(APawn* Pawn)
{
	IsMove = true;
	TargetAngle = GetAngle(Pawn->GetActorLocation()) > 90.0f ? -OpenAngle : OpenAngle;

	TimeLine->Play();
}

void ASEBaseDoor::Close()
{
	IsMove = true;

	TimeLine->Reverse();
}

void ASEBaseDoor::BeginPlay()
{
	Super::BeginPlay();

	check(ArrowComponent);

	if (CurveMovement != nullptr)
	{
		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindUFunction(this, FName("TimeLineUpdate"));

		TimeLine->AddInterpFloat(CurveMovement, TimelineCallback);

		FOnTimelineEventStatic TimelineFinishedCallback;
		TimelineFinishedCallback.BindUFunction(this, FName("StopMoveDoor"));

		TimeLine->SetTimelineFinishedFunc(TimelineFinishedCallback);
	}
}

void ASEBaseDoor::TimeLineUpdate(float Alpha)
{
	float NewYaw = FMath::Lerp(0.0f, TargetAngle, Alpha);

	DoorMesh->SetRelativeRotation(FRotator(0.0f, NewYaw, 0.0f));
}

void ASEBaseDoor::StopMoveDoor()
{
	IsMove = false;
	IsOpen = !IsOpen;

	Enable();
}

float ASEBaseDoor::GetAngle(FVector PawnLocation) const
{
	FVector ArrowLocation = ArrowComponent->GetComponentLocation();

	FVector NormalizedLocation = (PawnLocation - ArrowLocation).GetSafeNormal();
	FVector NormalizedForward = ArrowComponent->GetForwardVector().GetSafeNormal();

	return FMath::RadiansToDegrees(acosf(FVector::DotProduct(NormalizedLocation, NormalizedForward)));
}
