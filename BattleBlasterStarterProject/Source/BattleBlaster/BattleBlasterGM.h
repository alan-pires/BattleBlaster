// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Atank.h"
#include "ScreenMessage.h"

#include "BattleBlasterGM.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ABattleBlasterGM : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UScreenMessage> screenMessageClass;

	UScreenMessage* screenMessageWidget;

	UPROPERTY(EditAnywhere)
	float gameOverDelay = 3.0f;

	UPROPERTY(EditAnywhere)
	int32 countdownDelay = 3;
	int32 countdownSeconds;
	FTimerHandle countdownTH;

	Atank* tank;
	int32 towerCount;
	bool isGameOver;
	bool isVictory;

	void ActorDied(AActor* actor);
	void OnGameOverTimerTimeout();
	void OnCountdownTimerTimeout();
};
