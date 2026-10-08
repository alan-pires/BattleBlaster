// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	health = maxHealth;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnDamageTaken);
	
	AGameModeBase* gameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (gameMode)
		battleBlasterGM = Cast<ABattleBlasterGM>(gameMode);
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage > 0.0f)
	{
		health -= Damage;
		if (health == 0.0f)
		{
			if (battleBlasterGM)
				battleBlasterGM->ActorDied(DamagedActor);
		}
	}
}

