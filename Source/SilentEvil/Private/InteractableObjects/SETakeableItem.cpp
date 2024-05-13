// Silent Evil Game, All Rights Reserved.


#include "InteractableObjects/SETakeableItem.h"

#include "InventorySystem/SEBaseItem.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SEInventoryComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSETakeableItem, All, All);

ASETakeableItem::ASETakeableItem()
{
	DisableAfterInteract = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionProfileName(FName("NoCollision"));
}

void ASETakeableItem::Interact(APawn* Pawn)
{
	USEInventoryComponent* CharacterInventory = Pawn->FindComponentByClass<USEInventoryComponent>();
	if (CharacterInventory == nullptr)
	{
		StopCanInteract(Pawn);
		return;
	}

	bool Result = CharacterInventory->TryAddItem(Item, ItemCount);

	StopCanInteract(Pawn);
	if (Result && DisableAfterInteract)
	{
		ReceiveInteract(Pawn);
		Disable();
	}
}

void ASETakeableItem::Disable()
{
	MeshComponent->SetVisibility(false);
	Super::Disable();
}
