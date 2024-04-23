// Silent Evil Game, All Rights Reserved.


#include "Player/SEPlayerState.h"

#include "InventorySystem/SEItemData.h"

bool ASEPlayerState::AddItemToStorage(USEItemData* ItemData)
{
	if (ItemData == nullptr)
	{
		return false;
	}

	if (StorageItems.Contains(ItemData))
	{
		return false;
	}

	if (ItemData->GetItemCanStack())
	{
		for (int32 i = 0; i < StorageItems.Num(); i++)
		{
			if (StorageItems[i]->GetItem() != ItemData->GetItem() || StorageItems[i]->IsFull())
			{
				continue;
			}

			if (StorageItems[i]->GetCount() + ItemData->Count <= ItemData->GetItemMaxStackAmount())
			{
				StorageItems[i]->SetCount(StorageItems[i]->GetCount() + ItemData->Count);

				ItemData->MarkAsGarbage();
				return false;
			}
			else
			{
				ItemData->SetCount(StorageItems[i]->GetCount() + ItemData->Count - ItemData->GetItemMaxStackAmount());
				StorageItems[i]->SetCount(ItemData->GetItemMaxStackAmount());
			}
		}
	}

	StorageItems.Add(ItemData);
	ItemData->Rename(*ItemData->GetFName().ToString(), this, REN_None);

	return true;
}

void ASEPlayerState::RemoveItemFromStorage(USEItemData* ItemData)
{
	if (ItemData == nullptr)
	{
		return;
	}

	if (StorageItems.Contains(ItemData))
	{
		StorageItems.Remove(ItemData);
	}
}

TArray<USEItemData*> ASEPlayerState::GetItemsInStorage()
{
	return StorageItems;
}
