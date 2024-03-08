// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SEItemVisual.generated.h"

class USceneCaptureComponent2D;

UCLASS()
class SILENTEVIL_API ASEItemVisual : public AActor
{
	GENERATED_BODY()

public:
	ASEItemVisual();

	virtual void HorizontalInput(const float& Value);
	virtual void VerticalInput(const float& Value);
	virtual void Apply();
	virtual bool Cancel();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* ItemMeshComponent;

	virtual void BeginPlay() override;

};
