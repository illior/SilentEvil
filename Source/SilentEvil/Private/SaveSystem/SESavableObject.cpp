// Silent Evil Game, All Rights Reserved.

#include "SaveSystem/SESavableObject.h"

FSESaveDataRecord ISESavableObject::GetSaveDataRecord_Implementation()
{
	return FSESaveDataRecord();
}

void ISESavableObject::LoadFromSaveDataRecord_Implementation(FSESaveDataRecord InRecord)
{

}
