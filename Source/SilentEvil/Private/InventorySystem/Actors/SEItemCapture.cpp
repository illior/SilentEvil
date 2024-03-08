// Silent Evil Game, All Rights Reserved.

#include "InventorySystem/Actors/SEItemCapture.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Components/ArrowComponent.h"

ASEItemCapture::ASEItemCapture()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(SceneComponent);

	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>("CaptureComponent");
	CaptureComponent->SetupAttachment(SceneComponent);

	CaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	CaptureComponent->SetTickableWhenPaused(true);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("ArrowComponent");
	ArrowComponent->SetupAttachment(SceneComponent);
}

FTransform ASEItemCapture::GetTranformForNewItem()
{
	return ArrowComponent->GetComponentTransform();
}

void ASEItemCapture::AddItemToCapture(AActor* InActor)
{
	CaptureComponent->ShowOnlyActorComponents(InActor);
}

void ASEItemCapture::BeginPlay()
{
	Super::BeginPlay();
	
}
