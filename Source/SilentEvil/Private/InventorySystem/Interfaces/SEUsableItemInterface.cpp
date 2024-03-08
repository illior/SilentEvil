// Silent Evil Game, All Rights Reserved.


#include "InventorySystem/Interfaces/SEUsableItemInterface.h"

// Add default functionality here for any ISEUsableItemInterface functions that are not pure virtual.

bool ISEUsableItemInterface::CanUse(APawn* Pawn)
{
	return false;
}

void ISEUsableItemInterface::Use(APawn* Pawn)
{

}
