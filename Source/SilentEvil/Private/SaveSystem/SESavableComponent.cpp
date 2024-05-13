// Silent Evil Game, All Rights Reserved.


#include "SaveSystem/SESavableComponent.h"

// Add default functionality here for any ISESavableComponent functions that are not pure virtual.

FSESaveDataComponent ISESavableComponent::GetSaveData_Implementation()
{
	return FSESaveDataComponent();
}

void ISESavableComponent::LoadFromSaveData_Implementation(FSESaveDataComponent InRecord)
{
}
