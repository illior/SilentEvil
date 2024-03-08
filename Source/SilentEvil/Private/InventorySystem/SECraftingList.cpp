// Silent Evil Game, All Rights Reserved.


#include "InventorySystem/SECraftingList.h"

USEBaseItem* USECraftingList::CanCraft(USEBaseItem* Item1, USEBaseItem* Item2, int32& Count)
{
	FItemsCraft* FindedRow = Crafts.FindByPredicate([&](FItemsCraft Row) {
		return Row.Check(Item1, Item2);
	});

	if (FindedRow != nullptr)
	{
		Count = FindedRow->Count;
		return FindedRow->Result;
	}

	return nullptr;
}
