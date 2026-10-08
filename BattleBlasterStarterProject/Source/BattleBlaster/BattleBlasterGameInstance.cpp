// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UBattleBlasterGameInstance::ChangeLevel(int32 idx)
{
	if (idx && idx <= lastLevelIdx)
	{
		currentLevelIdx = idx;

		FString levelName = FString::Printf(TEXT("Level_%d"), currentLevelIdx);
		UGameplayStatics::OpenLevel(GetWorld(), *levelName);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Level does not exists"));
	}
}

void UBattleBlasterGameInstance::LoadNextLevel()
{
	if (currentLevelIdx < lastLevelIdx)
		ChangeLevel(currentLevelIdx + 1);
	else
		RestartGame();
}

void UBattleBlasterGameInstance::RestartCurrentLevel()
{
	ChangeLevel(currentLevelIdx);
}

void UBattleBlasterGameInstance::RestartGame()
{
	ChangeLevel(1);
}