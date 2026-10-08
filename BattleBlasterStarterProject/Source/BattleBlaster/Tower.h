// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Atank.h"
#include "Tower.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ATower : public ABasePawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	Atank* tank;

	UPROPERTY(EditAnywhere)
	double fireRange = 1000.0;

	UPROPERTY(EditAnywhere)
	float fireRate = 2.0f;

	void CheckFireCondition();
	bool isInRange();
	void HandleDestruction();
};
