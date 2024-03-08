// Silent Evil Game, All Rights Reserved.

#include "InventorySystem/Actors/SEItemVisual.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"

ASEItemVisual::ASEItemVisual()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(SceneComponent);

	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	ItemMeshComponent->SetupAttachment(SceneComponent);
	ItemMeshComponent->SetCollisionProfileName(FName("NoCollision"));

	ItemMeshComponent->bVisibleInSceneCaptureOnly = true;
}

void ASEItemVisual::HorizontalInput(const float& Value)
{
	//UE_LOG(LogTemp, Display, TEXT("ItemVisual Horizontal %f"), Value);

	FRotator Rotation = FRotator();
	Rotation.Roll += -Value * 5.0f;

	ItemMeshComponent->AddRelativeRotation(Rotation.Quaternion());
}

void ASEItemVisual::VerticalInput(const float& Value)
{
	//UE_LOG(LogTemp, Display, TEXT("ItemVisual Vertical %f"), Value);

	FRotator Rotation = FRotator();
	Rotation.Pitch += -Value * 5.0f;

	ItemMeshComponent->AddRelativeRotation(Rotation.Quaternion());
}

void ASEItemVisual::Apply()
{
	UE_LOG(LogTemp, Display, TEXT("ItemVisual Accept"));
}

bool ASEItemVisual::Cancel()
{
	UE_LOG(LogTemp, Display, TEXT("ItemVisual Cancel"));

	return true;
}

void ASEItemVisual::BeginPlay()
{
	Super::BeginPlay();
	
}
