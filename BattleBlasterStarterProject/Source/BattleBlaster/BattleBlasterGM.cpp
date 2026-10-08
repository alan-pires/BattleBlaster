// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGM.h"
#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "BattleBlasterGameInstance.h"

void ABattleBlasterGM::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> towers;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), towers);
	towerCount = towers.Num();

	UE_LOG(LogTemp, Display, TEXT("NUMBER OF TOWERS: %d"), towerCount);

	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (playerPawn)
	{
		tank = Cast<Atank>(playerPawn);
		if (!tank)
			UE_LOG(LogTemp, Display, TEXT("Error trying to get playerPawn"));
	}

	int32 i = 0;
	while (i < towerCount)
	{
		AActor* towerActor = towers[i];
		if (towerActor)
		{
			ATower* tower = Cast<ATower>(towerActor);
			if (tower)
			{
				tower->tank = tank;
				UE_LOG(LogTemp, Display, TEXT("%s setting the tank var"), *tower->GetActorNameOrLabel());
			}
		}
		i++;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		screenMessageWidget = CreateWidget<UScreenMessage>(PlayerController, screenMessageClass);
		if (screenMessageWidget)
		{
			screenMessageWidget->AddToPlayerScreen();
			screenMessageWidget->SetMessageText("Get Ready!");
		}
	}

	countdownSeconds = countdownDelay;
	GetWorldTimerManager().SetTimer(countdownTH, this, &ABattleBlasterGM::OnCountdownTimerTimeout, 1.0f, true);
}

void ABattleBlasterGM::OnCountdownTimerTimeout()
{
	countdownSeconds -= 1;

	if (countdownSeconds > 0)
	{
		screenMessageWidget->SetMessageText(FString::FromInt(countdownSeconds));
	}
	else if (countdownSeconds == 0)
	{
		screenMessageWidget->SetMessageText("Go!");
		tank->SetPlayerEnabled(true);
	}
	else
	{
		// CountdownSeconds is less than 0
		GetWorldTimerManager().ClearTimer(countdownTH);
		screenMessageWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}


void ABattleBlasterGM::ActorDied(AActor* actor)
{
	if (actor == tank)
	{
		tank->HandleDestruction();
		isGameOver = true;
	}
	else
	{
		ATower* tower = Cast<ATower>(actor);
		if (tower)
		{
			tower->HandleDestruction();
			towerCount--;


			if (towerCount == 0)
			{
				isGameOver = true;
				isVictory = true;
			}
		}
	}

	if (isGameOver)
	{
		FString GameOverString = isVictory ? "Victory!" : "Defeat!";
		screenMessageWidget->SetMessageText(GameOverString);
		screenMessageWidget->SetVisibility(ESlateVisibility::Visible);

		FTimerHandle GameOverTimerHandle;
		GetWorldTimerManager().SetTimer(GameOverTimerHandle, this, &ABattleBlasterGM::OnGameOverTimerTimeout, gameOverDelay, false);
	}
}

void ABattleBlasterGM::OnGameOverTimerTimeout()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UBattleBlasterGameInstance* BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance);
		if (BattleBlasterGameInstance)
		{
			if (isVictory)
			{
				// Load the next level
				BattleBlasterGameInstance->LoadNextLevel();
			}
			else
			{
				// Reload the current level
				BattleBlasterGameInstance->RestartCurrentLevel();
			}
		}
	}
}