// Silent Evil Game, All Rights Reserved.

#include "InventorySystem/SEBaseItem.h"

#include "InventorySystem/Actors/SEItemCapture.h"
#include "InventorySystem/Actors/SEItemVisual.h"
#include "Kismet/GameplayStatics.h"

ASEItemVisual* USEBaseItem::GetVisual(UWorld* World)
{
	if (ItemVisual != nullptr)
	{
		return ItemVisual;
	}

	if (World == nullptr)
	{
		return nullptr;
	}

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(World, ASEItemCapture::StaticClass(), Actors);
	if (Actors.Num() == 0)
	{
		return nullptr;
	}
	ASEItemCapture* ItemCapture = Cast<ASEItemCapture>(Actors[0]);

	FTransform ItemTransform = ItemCapture->GetTranformForNewItem();

	ItemVisual = World->SpawnActor<ASEItemVisual>(ItemVisualClass, ItemTransform);
	ItemCapture->AddItemToCapture(ItemVisual);

	return ItemVisual;
}
