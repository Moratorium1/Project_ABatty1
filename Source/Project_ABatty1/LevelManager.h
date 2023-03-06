// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelManager.generated.h"

class UProjectGameInstance;
class UGraphManager;
class ULevelGraph;
class AATile;

UCLASS()
class PROJECT_ABATTY1_API ULevelManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public: 

	UFUNCTION()
	void Initialise();

	UFUNCTION()
	void GenerateLevel(const int& LevelNumber);

	UFUNCTION()
	void SpawnLevel(const int& LevelNumber);

private:

	/* Reference to the GameInstance required to access the player type model */
	UPROPERTY()
	UProjectGameInstance* GameInstance = nullptr;

	UPROPERTY()
	UGraphManager* GraphManager = nullptr;

	UPROPERTY()
	TArray<ULevelGraph*> Levels;

};
