// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "SECoreTypes.h"
#include "SESavableObject.generated.h"

UINTERFACE(MinimalAPI)
class USESavableObject : public UInterface
{
	GENERATED_BODY()
};

class SILENTEVIL_API ISESavableObject
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SaveLoad")
	FSESaveDataRecord GetSaveDataRecord();
	virtual FSESaveDataRecord GetSaveDataRecord_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SaveLoad")
	void LoadFromSaveDataRecord(FSESaveDataRecord InRecord);
	virtual void LoadFromSaveDataRecord_Implementation(FSESaveDataRecord InRecord);
};
