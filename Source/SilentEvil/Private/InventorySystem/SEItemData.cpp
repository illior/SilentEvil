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
	return Item == nullptr ? FText() : Item->GetName();
}

FText USEItemData::GetItemDescription() const
{
	return Item == nullptr ? FText() : Item->GetDescription();
}

bool USEItemData::GetItemCanStack() const
{
	return Item == nullptr ? false : Item->GetCanStack();
}

int32 USEItemData::GetItemMaxStackAmount() const
{
	return Item == nullptr ? 1 : (Item->GetCanStack() ? Item->GetMaxStackAmount() : 1);
}

int32 USEItemData::GetItemSize() const
{
	return Item == nullptr ? 1 : Item->GetSize();
}

UTexture2D* USEItemData::GetItemIcon() const
{
	return Item == nullptr ? nullptr : Item->GetIcon();
}

ASEItemVisual* USEItemData::GetItemVisual() const
{
	return Item == nullptr ? nullptr : Item->GetVisual(GetWorld());
}

bool USEItemData::CanDrop() const
{
	return true;
}

bool USEItemData::IsWeapon() const
{
	return  Item == nullptr ? false : Item->IsA<USEBaseWeaponItem>();
}

bool USEItemData::IsFull() const
{
	return  Item == nullptr ? false : Count == Item->GetMaxStackAmount();
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
