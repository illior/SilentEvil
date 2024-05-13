// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SESubmenuWidget.generated.h"

UCLASS()
class SILENTEVIL_API USESubmenuWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void Open();
	virtual void Close();

	virtual void MoveVertical(int32 Offset);
	virtual void MoveHorizontal(int32 Offset);
	virtual void MoveAdditiveHorizotnal(int32 Offset);
	virtual void Apply();
	virtual bool Cancel();
};
