// Silent Evil Game, All Rights Reserved.

#include "InventorySystem/SEItemData.h"

#include "InventorySystem/SEBaseItem.h"
#include "InventorySystem/SEBaseWeaponItem.h"

USEBaseItem* USEItemData::GetItem() const
{
	return Item;
}

int32 USEItemData::GetCount() const
{
	return Count;
}

FIntPoint USEItemData::GetPosition() const
{
	return Position;
}

bool USEItemData::ContainsPoint(FIntPoint Point) const
{
	if (Item == nullptr)
	{
		return false;
	}

	return Point == Position || (GetItemSize() == 2 && FIntPoint(Position.X + 1, Position.Y) == Point);
}

FText USEItemData::GetItemName() const
{
	if (Item == nullptr)
	{
		return FText();
	}

	return Item->GetName();
}

FText USEItemData::GetItemDescription() const
{
	if (Item == nullptr)
	{
		return FText();
	}

	return Item->GetDescription();
}

bool USEItemData::GetItemCanStack() const
{
	if (Item == nullptr)
	{
		return false;
	}

	return  Item->GetCanStack();
}

int32 USEItemData::GetItemMaxStackAmount() const
{
	if (Item == nullptr)
	{
		return 1;
	}

	return Item->GetCanStack() ? Item->GetMaxStackAmount() : 1;
}

int32 USEItemData::GetItemSize() const
{
	if (Item == nullptr)
	{
		return 1;
	}

	return Item->GetSize();
}

UTexture2D* USEItemData::GetItemIcon() const
{
	if (Item == nullptr)
	{
		return nullptr;
	}

	return Item->GetIcon();
}

ASEItemVisual* USEItemData::GetItemVisual() const
{
	if (Item == nullptr)
	{
		return nullptr;
	}

	return Item->GetVisual(GetWorld());
}

bool USEItemData::CanDrop() const
{
	if (Item == nullptr)
	{
		return false;
	}

	return true;
}

bool USEItemData::IsWeapon() const
{
	if (Item == nullptr)
	{
		return false;
	}

	return  Item->IsA<USEBaseWeaponItem>();
}

bool USEItemData::IsFull() const
{
	if (Item == nullptr)
	{
		return false;
	}

	return Item->GetCanStack() ? Count == Item->GetMaxStackAmount() : true;
}

FString USEItemData::ToString() const
{
	if (Item == nullptr)
	{
		return "nullptr";
	}

	return FString::Printf(TEXT("%s, Position: %s, Count: %i"), *Item->GetName().ToString(), *Position.ToString(), Count);
}

void USEItemData::SetPosition(const int& X, const int& Y)
{
	Position.X = X;
	Position.Y = Y;

	OnDataChanged.Broadcast();
}

void USEItemData::SetCount(int32 NewCount)
{
	Count = FMath::Clamp(NewCount, 0.0f, GetItemMaxStackAmount());

	OnDataChanged.Broadcast();
}

void USEItemData::Drop()
{
	OnDrop.Broadcast();

	OnDrop.Clear();
	OnDataChanged.Clear();
}
