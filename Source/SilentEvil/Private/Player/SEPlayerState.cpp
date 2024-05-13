// Silent Evil Game, All Rights Reserved.


#include "Player/SEPlayerState.h"

#include "InventorySystem/SEBaseWeaponItem.h"
#include "InventorySystem/SEWeaponData.h"

FSESaveDataRecord ASEPlayerState::GetSaveDataRecord_Implementation()
{
	FSESaveDataRecord Record = FSESaveDataRecord();

	Record.ActorClass = GetClass();
	Record.ActorName = GetName();

	SaveItems.Empty();
	for (USEItemData* ItemData : StorageItems)
	{
		FSESaveItemData SaveItem = FSESaveItemData(ItemData);

		if (ItemData->IsWeapon())
		{
			USEWeaponData* WeaponData = Cast<USEWeaponData>(ItemData);

			SaveItem = FSESaveItemData(WeaponData);
		}

		SaveItems.Add(SaveItem);
	}

	FMemoryWriter Writer = FMemoryWriter(Record.BinaryData);
	FObjectAndNameAsStringProxyArchive Ar(Writer, false);
	Ar.ArIsSaveGame = true;

	Serialize(Ar);

	return Record;
}

void ASEPlayerState::LoadFromSaveDataRecord_Implementation(FSESaveDataRecord InRecord)
{
	FMemoryReader Reader = FMemoryReader(InRecord.BinaryData);
	FObjectAndNameAsStringProxyArchive Ar(Reader, false);
	Ar.ArIsSaveGame = true;

	Serialize(Ar);

	StorageItems.Empty();
	for (FSESaveItemData SaveItem : SaveItems)
	{
		if (SaveItem.Item->IsA<USEBaseWeaponItem>())
		{
			USEWeaponData* WeaponData = NewObject<USEWeaponData>(this);

			WeaponData->Item = SaveItem.Item;
			WeaponData->Count = SaveItem.Count;
			WeaponData->Position = SaveItem.Position;

			WeaponData->CurrentAmmoInClip = SaveItem.AmmoInClip;

			StorageItems.Add(WeaponData);
		}
		else
		{
			USEItemData* ItemData = NewObject<USEItemData>(this);

			ItemData->Item = SaveItem.Item;
			ItemData->Count = SaveItem.Count;
			ItemData->Position = SaveItem.Position;

			StorageItems.Add(ItemData);
		}
	}

	SaveItems.Empty();
}

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
