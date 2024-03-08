// Silent Evil Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SEHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SILENTEVIL_API USEHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USEHealthComponent();

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
	float CurrentHealth = 0.0f;
	bool IsDead = false;

	void SetHealth(float NewHealth);

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
