// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectGameInstance.generated.h"

class FXmlNode;
class UGraphManager;
class ULevelManager;
class AATile;

/* Holds values corresponding to Bartles Player Types */
USTRUCT()
struct FPlayerModel
{
	GENERATED_BODY()

	int Killer = 25;
	int Achiever = 25;
	int Socialiser = 25;
	int Explorer = 25;
};


UCLASS()
class PROJECT_ABATTY1_API UProjectGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	/* Initialises the Game Instance - Creating the Graph and Level manager subsystems */
	virtual void Init() override;

	/* Loads the player model */
	UFUNCTION()
	void LoadPlayerModel(const FString& PlayerModelFile);

	UFUNCTION()
	FPlayerModel GetPlayerModel();

	UFUNCTION()
	void IncreaseKillerType();

	UFUNCTION()
	void IncreaseAchieverType();

	UFUNCTION()
	void IncreaseSocialiserType();

	UFUNCTION()
	void IncreaseExplorerType();

	UFUNCTION()
	void NextLevel();

	/* Tile Information */

	UPROPERTY(EditAnywhere, Category = "Tile")
	TSubclassOf<AATile> TileClass;

	UPROPERTY(EditAnywhere, Category = "Tile")
	TSubclassOf<AATile> StartClass;

	UPROPERTY(EditAnywhere, Category = "Tile")
	TSubclassOf<AATile> HubClass;

	UPROPERTY(EditAnywhere, Category = "Tile")
	TSubclassOf<AATile> InjectClass;

	UPROPERTY(EditAnywhere, Category = "Tile")
	TSubclassOf<AATile> TreeClass;

	UPROPERTY(EditAnywhere, Category = "Tile")
	TSubclassOf<AATile> CycleClass;


	UPROPERTY(EditAnywhere, Category = "Tile")
	TSubclassOf<AATile> KillerClass;

	UPROPERTY(EditAnywhere, Category = "Tile")
	TSubclassOf<AATile> AchieverClass;

	UPROPERTY(EditAnywhere, Category = "Tile")
	TSubclassOf<AATile> SocialiserClass;

	UPROPERTY(EditAnywhere, Category = "Tile")
	TSubclassOf<AATile> ExplorerClass;


	UPROPERTY(EditAnywhere, Category = "Tile")
	int TileSize = 250;

	/* The Maximum number of attempts a composite can take to layout */
	UPROPERTY(EditAnywhere, Category = "LevelManager")
	int LayoutAttempts = 4;

	/* Level Number corresponds to the index of the LevelManagers Levels Array - incremented upon level completion */
	UPROPERTY()
	int LevelNumber = 0;

private:

	/* Struct that holds the values that represent the player's player type model */
	UPROPERTY()
	FPlayerModel PlayerModel;

	/* Contains all graph transformation functions  */
	UPROPERTY()
	UGraphManager* GraphManager;

	/* Holds all Level Graphs and calls on GraphManager to edit them */
	UPROPERTY()
	ULevelManager* LevelManager;

};
