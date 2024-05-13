// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SaveSystem/SESavableComponent.h"
#include "SEHealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SILENTEVIL_API USEHealthComponent : public UActorComponent, public ISESavableComponent
{
	GENERATED_BODY()

public:	
	USEHealthComponent();

	virtual FSESaveDataComponent GetSaveData_Implementation();
	virtual void LoadFromSaveData_Implementation(FSESaveDataComponent InRecord);

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	float GetMaxHealtht() const;
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	float GetCurrentHealth() const;
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	bool GetIsDead() const;

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void GetHeal(float Amount);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "1"))
	float MaxHealth = 100.0f;

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void ApplyDamage(float Amount);

	virtual void BeginPlay() override;

private:
	UPROPERTY(SaveGame)
	float CurrentHealth = 0.0f;
	UPROPERTY(SaveGame)
	bool IsDead = false;

	void SetHealth(float NewHealth);

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
