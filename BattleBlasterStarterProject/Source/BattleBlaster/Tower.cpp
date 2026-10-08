// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle fireTimer;
	GetWorldTimerManager().SetTimer(fireTimer, this, &ATower::CheckFireCondition, fireRate, true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (tank && isInRange())
		RotateTurret(tank->GetActorLocation());
}

void ATower::CheckFireCondition()
{
	if (tank && tank->isAlive && isInRange())
		Fire();
}

bool ATower::isInRange()
{
	double dist = FVector::Dist(GetActorLocation(), tank->GetActorLocation());

	if (dist < fireRange)
		return true;
	return false;
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();

	UE_LOG(LogTemp, Display, TEXT("Tower handle destruction !!"));
	Destroy();
}
