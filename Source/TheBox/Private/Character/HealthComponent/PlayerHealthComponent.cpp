#include "Character/HealthComponent/PlayerHealthComponent.h"

// Sets default values for this component's properties
UPlayerHealthComponent::UPlayerHealthComponent() : MaxArmor(100.F), CurrentArmor(MaxArmor), MaxHealth(100.F), CurrentHealth(MaxHealth)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UPlayerHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UPlayerHealthComponent::TakeDamage);
	//GetOwner()->OnTakeRadialDamage.AddDynamic(this, &UPlayerHealthComponent::TakeRadialDamage);
}

// Called every frame
void UPlayerHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerHealthComponent::UpdatePlayerStats_Implementation(float HealthPickupValue, float ArmorPickupValue)
{
	if (CurrentArmor > 0.F)
	{
		CurrentArmor += ArmorPickupValue;

		if (CurrentArmor >= MaxArmor)
			CurrentArmor = FMath::Clamp(CurrentArmor, 0.F, MaxArmor);

		PlayerHealthAndArmorUpdate.Broadcast(CurrentArmor, CurrentHealth);
	}

	else if (CurrentHealth > 0.F)
	{
		CurrentHealth += HealthPickupValue;

		if (CurrentHealth >= MaxHealth)
			CurrentHealth = FMath::Clamp(CurrentHealth, 0.F, MaxHealth);

		PlayerHealthAndArmorUpdate.Broadcast(CurrentArmor, CurrentHealth);
	}
}

void UPlayerHealthComponent::ChangeHealthAndArmor(float ChangeAmount)
{
	if (CurrentArmor > 0.F)
	{
		CurrentArmor -= ChangeAmount;

		if (CurrentArmor <= 0.F)
			CurrentArmor = FMath::Clamp(CurrentArmor, 0.F, MaxArmor);
	}

	else if (CurrentHealth > 0.F && CurrentArmor <= 0.F)
	{
		CurrentHealth -= ChangeAmount;
	
		if (!IsPlayerAlive())
		{
			if (CurrentHealth <= 0.F)
				CurrentHealth = FMath::Clamp(CurrentHealth, 0.F, MaxHealth);

			PlayerDeath.Broadcast();
		}
	}
}

void UPlayerHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	ChangeHealthAndArmor(Damage);

	PlayerHealthAndArmorUpdate.Broadcast(CurrentArmor, CurrentHealth);
}

void UPlayerHealthComponent::TakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	ChangeHealthAndArmor(Damage);

	PlayerHealthAndArmorUpdate.Broadcast(CurrentArmor, CurrentHealth);
}

bool UPlayerHealthComponent::IsPlayerAlive() { return CurrentHealth > 0.F; }

