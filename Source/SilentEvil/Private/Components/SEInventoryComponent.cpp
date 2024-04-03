// Silent Evil Game, All Rights Reserved.

#include "Components/SEInventoryComponent.h"

#include "InventorySystem/SEReadableRecord.h"
#include "InventorySystem/SEItemData.h"
#include "InventorySystem/SEWeaponData.h"
#include "InventorySystem/SEBaseItem.h"
#include "InventorySystem/SEBaseWeaponItem.h"
#include "InventorySystem/SEAmmoItem.h"
#include "InventorySystem/Interfaces/SEUsableItemInterface.h"

#include "InventorySystem/SECraftingList.h"
#include "Engine/DataTable.h"

#include "InputActionValue.h"

DEFINE_LOG_CATEGORY_STATIC(LogCharacterInventory, All, All);

USEInventoryComponent::USEInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

FIntPoint USEInventoryComponent::GetMaxSize() const
{
	return MaxSize;
}

int32 USEInventoryComponent::GetCurrentSlotsCount() const
{
	return CurrentSlotsCount;
}

TIndexedContainerIterator<const TArray<USEItemData*, FDefaultAllocator>, USEItemData* const, int32> USEInventoryComponent::GetItemsIterator() const
{
	return Items.CreateConstIterator();
}

ESlotState USEInventoryComponent::GetSlotState(const int32& X, const int32& Y) const
{
	if (ItemsMatrix == nullptr)
	{
		return ESlotState::Block;
	}

	if (X < 0 || X >= MaxSize.X ||
		Y < 0 || Y >= MaxSize.Y)
	{
		return ESlotState::Block;
	}

	return ItemsMatrix[Y][X];
}

ESlotState USEInventoryComponent::GetSlotState(const FIntPoint& Position) const
{
	return GetSlotState(Position.X, Position.Y);
}

ESlotState USEInventoryComponent::GetSlotState(const int32& Index) const
{
	return GetSlotState(Index % MaxSize.X, Index / MaxSize.X);
}

USEItemData* USEInventoryComponent::GetItemDataByPosition(const int32& X, const int32& Y)
{
	for (USEItemData* ItemData : Items)
	{
		if (ItemData->ContainsPoint(FIntPoint(X, Y)))
		{
			return ItemData;
		}
	}

	return nullptr;
}

bool USEInventoryComponent::MoveItemTo(USEItemData* ItemData, const int32& X, const int32& Y)
{
	const ESlotState SlotStateOnPosition = GetSlotState(X, Y);
	if (SlotStateOnPosition == ESlotState::Block || !Items.Contains(ItemData))
	{
		return false;
	}

	USEItemData* ItemDataOnPosition = GetItemDataByPosition(X, Y);
	if (TryCraft(ItemData, ItemDataOnPosition))
	{
		return true;
	}

	if (TryCombine(ItemData, ItemDataOnPosition))
	{
		return true;
	}

	if (ItemData->Item->GetSize() == 1)
	{
		return MoveShortItem(ItemData, X, Y);
	}
	else if (ItemData->Item->GetSize() == 2)
	{
		return MoveLongItem(ItemData, X, Y);
	}

	return false;
}

void USEInventoryComponent::UseItem(USEItemData* ItemData)
{
	if (!Items.Contains(ItemData))
	{
		return;
	}

	ISEUsableItemInterface* UsableItem = Cast<ISEUsableItemInterface>(ItemData->GetItem());
	if (UsableItem == nullptr)
	{
		return;
	}

	UsableItem->Use(Cast<APawn>(GetOwner()));
	if (ItemData->GetItemCanStack())
	{
		if (ItemData->GetCount() == 1)
		{
			DestroyItem(ItemData);
		}
		else
		{
			ItemData->SetCount(ItemData->GetCount() - 1);
		}
	}
	else
	{
		DestroyItem(ItemData);
	}
}

bool USEInventoryComponent::DropItem(USEItemData* ItemData)
{
	if (Items.Contains(ItemData))
	{
		if (ItemData->Item->IsA<USEAmmoItem>())
		{
			USEAmmoItem* AmmoItem = Cast<USEAmmoItem>(ItemData->Item);
			if (AmmoItem != nullptr)
			{
				Ammunation[AmmoItem->GetAmmoType()] -= ItemData->GetCount();
			}
		}

		DestroyItem(ItemData);
	
		return true;
	}

	return false;
}

bool USEInventoryComponent::TryAddItem(USEBaseItem* Item, int32& Count)
{
	if (Item == nullptr)
	{
		return false;
	}

	if (Item->IsA<USEAmmoItem>())
	{
		USEAmmoItem* AmmoItem = Cast<USEAmmoItem>(Item);
		if (!Ammunation.Contains(AmmoItem->GetAmmoType()))
		{
			Ammunation.Add(AmmoItem->GetAmmoType(), Count);
		}
		else
		{
			Ammunation[AmmoItem->GetAmmoType()] += Count;
		}
	}

	if (Item->GetCanStack())
	{
		for (USEItemData* ItemData : Items)
		{
			if (ItemData->GetItem() != Item || ItemData->IsFull())
			{
				continue;
			}

			if (ItemData->GetCount() + Count <= Item->GetMaxStackAmount())
			{
				ItemData->SetCount(ItemData->GetCount() + Count);

				return true;
			}
			else
			{
				Count = ItemData->GetCount() + Count - Item->GetMaxStackAmount();
				ItemData->SetCount(Item->GetMaxStackAmount());
			}
		}
	}

	for (int32 i = 0; i < MaxSize.Y; i++)
	{
		for (int32 j = 0; j < MaxSize.X; j++)
		{
			if (ItemsMatrix[i][j] != ESlotState::Void)
			{
				continue;
			}

			if (Item->GetSize() == 2 && GetSlotState(j + 1, i) != ESlotState::Void)
			{
				continue;
			}

			USEItemData* NewItem = CreateItemData(Item, j, i, Count);
			TryAddAmmo(NewItem);

			UE_LOG(LogCharacterInventory, Display, TEXT("Add item: %s"), *NewItem->Position.ToString());
			return true;
		}
	}

	if (Item->IsA<USEAmmoItem>())
	{
		USEAmmoItem* AmmoItem = Cast<USEAmmoItem>(Item);
		Ammunation[AmmoItem->GetAmmoType()] -= Count;
	}
	return false;
}

void USEInventoryComponent::AddRecord(FName RowName)
{
	/*if (Records.Contains(RowName))
	{
		return;
	}*/

	if (RecordsTable == nullptr)
	{
		return;
	}

	FSEReadableRecord* Item = RecordsTable->FindRow<FSEReadableRecord>(RowName, "", false);
	if (Item == nullptr)
	{
		return;
	}

	Records.Add(RowName);
}

TArray<FSEReadableRecord> USEInventoryComponent::GetRecords()
{
	if (RecordsTable == nullptr)
	{
		return TArray<FSEReadableRecord>();
	}

	TArray<FSEReadableRecord> Rows;

	for (int i = 0; i < Records.Num(); i++)
	{
		FSEReadableRecord* Item = RecordsTable->FindRow<FSEReadableRecord>(Records[i], "", false);

		if (Item != nullptr)
		{
			Rows.Add(*Item);
		}
	}

	return Rows;
}

void USEInventoryComponent::ReloadEquipWeapon()
{
	if (CurrentWeapon == nullptr)
	{
		return;
	}

	EAmmoType EquipWeaponAmmoType = CurrentWeapon->GetAmmoType();
	int32 EquipWeaponAmmo = CurrentWeapon->GetCurrentAmmoInClip();
	if (CurrentWeapon->IsFullAmmo() || GetCountAmmoType(EquipWeaponAmmoType) == 0)
	{
		return;
	}

	CurrentWeapon->SetCurrentAmmoInClip(FMath::Clamp(Ammunation[EquipWeaponAmmoType] + EquipWeaponAmmo, 1, CurrentWeapon->GetMaxAmmoInClip()));
	Ammunation[EquipWeaponAmmoType] -= CurrentWeapon->GetCurrentAmmoInClip() - EquipWeaponAmmo;
	RemoveAmmo(EquipWeaponAmmoType, CurrentWeapon->GetCurrentAmmoInClip() - EquipWeaponAmmo);
}

void USEInventoryComponent::SetEquipWeapon(USEItemData* ItemData)
{
	if (ItemData == nullptr)
	{
		return;
	}

	if (!ItemData->IsWeapon() || !Items.Contains(ItemData))
	{
		return;
	}

	USEWeaponData* WeaponData = Cast<USEWeaponData>(ItemData);
	if (WeaponData == nullptr)
	{
		return;
	}

	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->SetEquip(false);
	}

	CurrentWeapon = WeaponData;
	WeaponData->SetEquip(true);
}

USEWeaponData* USEInventoryComponent::GetEquipWeapon() const
{
	return CurrentWeapon;
}

void USEInventoryComponent::SetFastAccessWeapon(USEItemData* ItemData, int32 Index)
{
	if (ItemData == nullptr)
	{
		if (FastAccessWeapons.IsValidIndex(Index))
		{
			FastAccessWeapons[Index] = nullptr;
		}

		return;
	}

	if (!ItemData->IsWeapon() || !Items.Contains(ItemData) || !FastAccessWeapons.IsValidIndex(Index))
	{
		return;
	}

	USEWeaponData* WeaponData = Cast<USEWeaponData>(ItemData);
	if (WeaponData == nullptr)
	{
		return;
	}

	if (FastAccessWeapons.Contains(WeaponData))
	{
		int32 WeaponIndex = FastAccessWeapons.IndexOfByKey(WeaponData);

		FastAccessWeapons[WeaponIndex] = FastAccessWeapons[Index];
		if (FastAccessWeapons[Index] != nullptr)
		{
			FastAccessWeapons[Index]->SetFastAccessIndex(WeaponIndex);
		}
	}

	FastAccessWeapons[Index] = WeaponData;
	WeaponData->SetFastAccessIndex(Index);
}

USEWeaponData* USEInventoryComponent::GetFastAccessWeapon(int32 Index) const
{
	if (FastAccessWeapons.IsValidIndex(Index))
	{
		return FastAccessWeapons[Index];
	}

	return nullptr;
}

int32 USEInventoryComponent::GetCountAmmoType(EAmmoType AmmoType)
{
	if (!Ammunation.Contains(AmmoType))
	{
		return 0;
	}

	return Ammunation[AmmoType];
}

void USEInventoryComponent::FastAccessItem(const FInputActionValue& Value)
{
	int32 Index = (int32)Value.Get<float>() - 1;
	if (!FastAccessWeapons.IsValidIndex(Index))
	{
		return;
	}

	if (FastAccessWeapons[Index] == nullptr || FastAccessWeapons[Index] == CurrentWeapon)
	{
		return;
	}

	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->SetEquip(false);
	}

	CurrentWeapon = FastAccessWeapons[Index];
	CurrentWeapon->SetEquip(true);

	UE_LOG(LogCharacterInventory, Display, TEXT("Current Weapon %s"), *CurrentWeapon->GetItemName().ToString());
}

void USEInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void USEInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DestroyMatrix();

	GetWorld();

	Super::EndPlay(EndPlayReason);
}

void USEInventoryComponent::Initialize()
{
	CreateMatrix();

	for (int32 i = 0; i < Items.Num(); i++)
	{
		SetSlotState(Items[i]->Position.X, Items[i]->Position.Y, ESlotState::Item);

		if (Items[i]->Item->GetSize() == 2)
		{
			SetSlotState(Items[i]->Position.X + 1, Items[i]->Position.Y, ESlotState::Item);
		}

		TryAddAmmo(Items[i]);
	}

	FastAccessWeapons.Empty();
	FastAccessWeapons.Add(nullptr);
	FastAccessWeapons.Add(nullptr);
	FastAccessWeapons.Add(nullptr);
	FastAccessWeapons.Add(nullptr);
}

USEItemData* USEInventoryComponent::CreateItemData(USEBaseItem* Item, const int32& X, const int32& Y, const int32& Count)
{
	USEItemData* NewItemData = nullptr;

	if (Item->IsA<USEBaseWeaponItem>())
	{
		USEWeaponData* WeaponData = NewObject<USEWeaponData>(this);
		WeaponData->Item = Item;
		WeaponData->Count = 1;
		WeaponData->SetCurrentAmmoInClip(Count);

		NewItemData = WeaponData;
	}
	else
	{
		USEItemData* ItemData = NewObject<USEItemData>(this);
		ItemData->Item = Item;
		ItemData->SetCount(Count);

		NewItemData = ItemData;
	}


	SetItemPosition(NewItemData, X, Y);
	Items.Add(NewItemData);

	return NewItemData;
}

void USEInventoryComponent::DestroyItem(USEItemData* ItemData)
{
	if (ItemData == nullptr)
	{
		return;
	}

	SetSlotState(ItemData->Position.X, ItemData->Position.Y, ESlotState::Void);

	if (ItemData->GetItemSize() == 2)
	{
		SetSlotState(ItemData->Position.X + 1, ItemData->Position.Y, ESlotState::Void);
	}

	ItemData->Drop();
	Items.Remove(ItemData);

	ItemData->MarkAsGarbage();
}

bool USEInventoryComponent::MoveShortItem(USEItemData* ItemData, const int32& X, const int32& Y)
{
	const ESlotState SlotStateOnPosition = GetSlotState(X, Y);

	if (SlotStateOnPosition == ESlotState::Void)
	{
		SetSlotState(ItemData->Position.X, ItemData->Position.Y, ESlotState::Void);
		SetItemPosition(ItemData, X, Y);

		return true;
	}

	USEItemData* ItemDataOnPosition = GetItemDataByPosition(X, Y);
	if (ItemDataOnPosition == nullptr)
	{
		return false;
	}

	if (ItemDataOnPosition->GetItemSize() == 1)
	{
		ItemDataOnPosition->SetPosition(ItemData->Position.X, ItemData->Position.Y);

		ItemData->SetPosition(X, Y);

		return true;
	}
	FIntPoint ItemDataPosition = ItemDataOnPosition->Position;

	const ESlotState SlotStateItemNext = GetSlotState(ItemData->Position.X + 1, ItemData->Position.Y);
	if (SlotStateItemNext == ESlotState::Void)
	{
		SetItemPosition(ItemDataOnPosition, ItemData->Position.X, ItemData->Position.Y);

		ItemData->SetPosition(ItemDataPosition.X, ItemDataPosition.Y);
		SetSlotState(ItemDataPosition.X + 1, ItemDataPosition.Y, ESlotState::Void);
		return true;
	}

	return false;
}

bool USEInventoryComponent::MoveLongItem(USEItemData* ItemData, const int32& X, const int32& Y)
{
	SetSlotState(ItemData->Position.X, ItemData->Position.Y, ESlotState::Void);
	SetSlotState(ItemData->Position.X + 1, ItemData->Position.Y, ESlotState::Void);

	const ESlotState SlotStateOnPosition = GetSlotState(X, Y);
	const ESlotState SlotStateNextOnPosition = GetSlotState(X + 1, Y);
	const ESlotState SlotStatePrevOnPosition = GetSlotState(X - 1, Y);

	if (SlotStateOnPosition == ESlotState::Void && SlotStateNextOnPosition == ESlotState::Void)
	{
		SetItemPosition(ItemData, X, Y);

		return true;
	}

	USEItemData* ItemDataOnPosition = GetItemDataByPosition(X, Y);
	USEItemData* ItemDataNextOnPosition = GetItemDataByPosition(X + 1, Y);

	if (ItemData->Position.Y == Y && ItemData->Position.X + 1 == X && ItemDataNextOnPosition != nullptr && ItemDataNextOnPosition->GetItemSize() == 1)
	{
		SetItemPosition(ItemDataNextOnPosition, ItemData->Position.X, ItemData->Position.Y);
		SetItemPosition(ItemData, X, Y);

		return true;
	}

	if (SlotStateOnPosition == ESlotState::Void && ItemDataNextOnPosition != nullptr)
	{
		if (ItemDataNextOnPosition->GetItemSize() == 1)
		{
			SetItemPosition(ItemDataNextOnPosition, ItemData->Position.X, ItemData->Position.Y);
			SetItemPosition(ItemData, X, Y);

			return true;
		}
	}

	if (ItemDataOnPosition != nullptr)
	{
		if (ItemDataOnPosition->GetItemSize() == 2)
		{
			SetItemPosition(ItemDataOnPosition, ItemData->Position.X, ItemData->Position.Y);
			SetItemPosition(ItemData, X, Y);

			return true;
		}

		if (SlotStateNextOnPosition == ESlotState::Void)
		{
			if (ItemData->Position.Y == Y && ItemData->Position.X - 1 == X)
			{
				SetItemPosition(ItemDataOnPosition, ItemData->Position.X + 1, ItemData->Position.Y);
			}
			else
			{
				SetItemPosition(ItemDataOnPosition, ItemData->Position.X, ItemData->Position.Y);
			}
			
			SetItemPosition(ItemData, X, Y);

			return true;
		}

		if (ItemDataNextOnPosition != nullptr && ItemDataNextOnPosition->GetItemSize() == 1)
		{
			SetItemPosition(ItemDataOnPosition, ItemData->Position.X, ItemData->Position.Y);
			SetItemPosition(ItemDataNextOnPosition, ItemData->Position.X + 1, ItemData->Position.Y);


			SetItemPosition(ItemData, X, Y);

			return true;
		}
	}

	SetSlotState(ItemData->Position.X, ItemData->Position.Y, ESlotState::Item);
	SetSlotState(ItemData->Position.X + 1, ItemData->Position.Y, ESlotState::Item);

	return false;
}

void USEInventoryComponent::SetSlotState(const int32& X, const int32& Y, ESlotState State)
{
	if (X < 0 || X >= MaxSize.X ||
		Y < 0 || Y >= MaxSize.Y)
	{
		return;
	}

	ItemsMatrix[Y][X] = State;
}

void USEInventoryComponent::SetItemPosition(USEItemData* Item, const int32& X, const int32& Y)
{
	Item->SetPosition(X, Y);

	SetSlotState(Item->Position.X, Item->Position.Y, ESlotState::Item);
	if (Item->GetItemSize() == 2)
	{
		SetSlotState(Item->Position.X + 1, Item->Position.Y, ESlotState::Item);
	}
}

bool USEInventoryComponent::TryCraft(USEItemData* ItemData1, USEItemData* ItemData2)
{
	if (CraftingList == nullptr || ItemData1 == nullptr || ItemData2 == nullptr)
	{
		return false;
	}

	int32 ItemCount;
	USEBaseItem* Result = CraftingList->CanCraft(ItemData1->Item, ItemData2->Item, ItemCount);
	
	if (Result != nullptr)
	{
		FIntPoint NewItemPosition = ItemData2->Position;

		DestroyItem(ItemData1);
		DestroyItem(ItemData2);

		USEItemData* NewItemData = CreateItemData(Result, NewItemPosition.X, NewItemPosition.Y, ItemCount);
		TryAddAmmo(NewItemData);

		OnItemCrafted.Broadcast(NewItemData);

		return true;
	}

	return false;
}

bool USEInventoryComponent::TryCombine(USEItemData* ItemData1, USEItemData* ItemData2)
{
	if (ItemData1 == nullptr || ItemData2 == nullptr)
	{
		return false;
	}

	if(ItemData1->Item != ItemData2->Item || !ItemData1->GetItemCanStack())
	{
		return false;
	}

	UE_LOG(LogCharacterInventory, Display, TEXT("TryCombine"));
	if ((ItemData2->Count + ItemData1->Count) > ItemData1->GetItemMaxStackAmount())
	{
		ItemData1->SetCount((ItemData1->Count + ItemData2->Count) - ItemData1->GetItemMaxStackAmount());
		ItemData2->SetCount(ItemData2->GetItemMaxStackAmount());

		return true;
	}

	ItemData2->SetCount(ItemData1->Count + ItemData2->Count);
	DestroyItem(ItemData1);

	return true;
}

void USEInventoryComponent::TryAddAmmo(USEItemData* ItemData)
{
	if (!ItemData->Item->IsA<USEAmmoItem>())
	{
		return;
	}

	USEAmmoItem* AmmoItem = Cast<USEAmmoItem>(ItemData->Item);
	if (AmmoItem == nullptr)
	{
		return;
	}

	if (Ammunation.Contains(AmmoItem->GetAmmoType()))
	{
		Ammunation[AmmoItem->GetAmmoType()] += ItemData->GetCount();
	}
	else
	{
		Ammunation.Add(AmmoItem->GetAmmoType(), ItemData->GetCount());
	}
}

void USEInventoryComponent::RemoveAmmo(EAmmoType AmmoType, int32 Count)
{
	TArray<USEItemData*> ItemsToDestroy;

	for (USEItemData* ItemData : Items)
	{
		if (ItemData->Item->IsA<USEAmmoItem>())
		{
			USEAmmoItem* AmmoItem = Cast<USEAmmoItem>(ItemData->Item);
			if (AmmoItem->GetAmmoType() != AmmoType)
			{
				continue;
			}

			if (ItemData->GetCount() == Count)
			{
				ItemsToDestroy.Add(ItemData);

				break;
			}
			if (ItemData->GetCount() > Count)
			{
				ItemData->SetCount(ItemData->GetCount() - Count);

				break;
			}
			else
			{
				Count -= ItemData->GetCount();
				ItemsToDestroy.Add(ItemData);
			}
		}
	}

	if (ItemsToDestroy.Num() != 0)
	{
		for (USEItemData* ItemData : ItemsToDestroy)
		{
			DestroyItem(ItemData);
		}
		
		ItemsToDestroy.Empty();
	}
}

void USEInventoryComponent::CreateMatrix()
{
	ItemsMatrix = new ESlotState * [MaxSize.Y];

	for (int32 i = 0; i < MaxSize.Y; i++)
	{
		ItemsMatrix[i] = new ESlotState[MaxSize.X];

		for (int32 j = 0; j < MaxSize.X; j++)
		{
			if ((i * MaxSize.X + j) < CurrentSlotsCount)
			{
				ItemsMatrix[i][j] = ESlotState::Void;
				continue;
			}

			ItemsMatrix[i][j] = ESlotState::Block;
		}
	}
}

void USEInventoryComponent::DestroyMatrix()
{
	for (int32 i = 0; i < MaxSize.Y; i++)
	{
		delete[] ItemsMatrix[i];
	}
	delete[] ItemsMatrix;

	ItemsMatrix = nullptr;
}

void USEInventoryComponent::LogInfo()
{
	/*for (int i = 0; i < Items.Num(); i++)
	{
		UE_LOG(LogCharacterInventory, Display, TEXT("%s"), *Items[i]->Item->GetName().ToString());
	}*/

	/*for (int32 i = 0; i < MaxSize.Y; i++)
	{
		FString str = "";
		for (int32 j = 0; j < MaxSize.X; j++)
		{
			str += FString::Printf(TEXT("%s "), *UEnum::GetValueAsString(GetSlotState(j, i)));
		}

		UE_LOG(LogCharacterInventory, Display, TEXT("%s"), *str);
	}*/

	/*if (CurrentWeapon != nullptr)
	{
		UE_LOG(LogCharacterInventory, Display, TEXT("Current weapon %s"), *CurrentWeapon->Item->GetName().ToString());
	}
	else
	{
		UE_LOG(LogCharacterInventory, Display, TEXT("Current weapon nullptr"));
	}*/
	
	/*UE_LOG(LogCharacterInventory, Display, TEXT("Weapons %i"), FastAccessWeapons.Num());
	for (int i = 0; i < FastAccessWeapons.Num(); i++)
	{
		if (FastAccessWeapons[i] == nullptr)
		{
			UE_LOG(LogCharacterInventory, Display, TEXT("nullptr"));
			continue;
		}

		UE_LOG(LogCharacterInventory, Display, TEXT("%s"), *FastAccessWeapons[i]->Item->GetName().ToString());
	}*/
	
	// g_herb|r_herb
	//CraftingTable->Find
	//CraftingTable->ForeachRow();
}
