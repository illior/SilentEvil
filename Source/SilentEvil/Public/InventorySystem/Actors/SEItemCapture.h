// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SEItemCapture.generated.h"

class USceneCaptureComponent2D;
class UArrowComponent;

UCLASS()
class SILENTEVIL_API ASEItemCapture : public AActor
{
	GENERATED_BODY()
	
public:
	ASEItemCapture();

	UFUNCTION(BlueprintCallable, Category = "ItemData")
	void AddItemToCapture(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "ItemData")
	FTransform GetTranformForNewItem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USceneCaptureComponent2D* CaptureComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ArrowComponent;

	virtual void BeginPlay() override;
};
