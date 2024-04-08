#pragma once

#include "CoreMinimal.h"
#include "SEReadableRecord.generated.h"

USTRUCT(BlueprintType)
struct FSEReadableRecord : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Record")
	FText Title;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Record")
	TArray<FText> Pages;
};