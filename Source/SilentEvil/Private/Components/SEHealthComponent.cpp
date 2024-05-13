// Silent Evil Game, All Rights Reserved.


#include "Components/SEHealthComponent.h"

USEHealthComponent::USEHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

FSESaveDataComponent USEHealthComponent::GetSaveData_Implementation()
{
	FSESaveDataComponent Record = FSESaveDataComponent();

	Record.ComponentName = GetName();

	FMemoryWriter Writer = FMemoryWriter(Record.BinaryData);
	FObjectAndNameAsStringProxyArchive Ar(Writer, false);
	Ar.ArIsSaveGame = true;

	Serialize(Ar);

	return Record;
}

void USEHealthComponent::LoadFromSaveData_Implementation(FSESaveDataComponent InRecord)
{
	FMemoryReader Reader = FMemoryReader(InRecord.BinaryData);
	FObjectAndNameAsStringProxyArchive Ar(Reader, false);
	Ar.ArIsSaveGame = true;

	Serialize(Ar);
}

float USEHealthComponent::GetMaxHealtht() const
{
	return MaxHealth;
}

float USEHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

bool USEHealthComponent::GetIsDead() const
{
	return IsDead;
}

void USEHealthComponent::GetHeal(float Amount)
{
	if (Amount < 0)
	{
		return;
	}

	SetHealth(CurrentHealth + Amount);
}

void USEHealthComponent::ApplyDamage(float Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	SetHealth(CurrentHealth - Amount);

	if (FMath::IsNearlyZero(CurrentHealth))
	{
		IsDead = true;
	}
}

void USEHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	IsDead = false;

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USEHealthComponent::OnTakeAnyDamage);
	}
}

void USEHealthComponent::SetHealth(float NewHealth)
{
	CurrentHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
}

void USEHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	ApplyDamage(Damage);
}
