// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "SECoreTypes.h"
#include "SESavableComponent.generated.h"

UINTERFACE(MinimalAPI)
class USESavableComponent : public UInterface
{
	GENERATED_BODY()
};

class SILENTEVIL_API ISESavableComponent
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SaveLoad")
	FSESaveDataComponent GetSaveData();
	virtual FSESaveDataComponent GetSaveData_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SaveLoad")
	void LoadFromSaveData(FSESaveDataComponent InRecord);
	virtual void LoadFromSaveData_Implementation(FSESaveDataComponent InRecord);
};
