// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectGameInstance.generated.h"

class FXmlNode;
class UGraphManager;
class ULevelManager;
class URoomManager;
class AATile;
class ATile;

/* Holds values corresponding to Bartles Player Types */
USTRUCT(BlueprintType)
struct FPlayerModel
{
	GENERATED_BODY()

	UPROPERTY()
	int Maximum = 50;

	UPROPERTY()
	int Minimum = 0;

	UPROPERTY(BlueprintReadWrite, Category="PlayerType")
	int Killer = 25;

	UPROPERTY(BlueprintReadWrite, Category = "PlayerType")
	int Achiever = 25;

	UPROPERTY(BlueprintReadWrite, Category = "PlayerType")
	int Socialiser = 25;

	UPROPERTY(BlueprintReadWrite, Category = "PlayerType")
	int Explorer = 25;
};


UCLASS()
class PROJECT_ABATTY1_API UProjectGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	/* Initialises the Game Instance - Creating the Graph and Level manager subsystems */
	virtual void Init() override;

	/* Player Model Functions */

	/* Loads the player model */
	UFUNCTION()
	void LoadPlayerModel(const FString& PlayerModelFile);

	/* Returns the player model */
	UFUNCTION()
	FPlayerModel GetPlayerModel();

	/* Increases the player type decreases the opposing player type */
	UFUNCTION(BlueprintCallable)
	void IncreaseKillerType();

	/* Returns the value of the player type */
	UFUNCTION(BlueprintCallable)
	int GetKillerType();

	/* Increases the player type decreases the opposing player type */
	UFUNCTION(BlueprintCallable)
	void IncreaseAchieverType();

	/* Returns the value of the player type */
	UFUNCTION(BlueprintCallable)
	int GetAchieverType();

	/* Increases the player type decreases the opposing player type */
	UFUNCTION(BlueprintCallable)
	void IncreaseSocialiserType();

	/* Returns the value of the player type */
	UFUNCTION(BlueprintCallable)
	int GetSocialiserType();

	/* Increases the player type decreases the opposing player type */
	UFUNCTION(BlueprintCallable)
	void IncreaseExplorerType();

	/* Returns the value of the player type */
	UFUNCTION(BlueprintCallable)
	int GetExplorerType();

	/*  */

	/* Holds rules input by the player */
	UPROPERTY()
	TArray<FString> InjectionQueue;

	/* Places a quest rule into the Injection Queue */
	UFUNCTION(BlueprintCallable)
	void InjectQuest();

	/* Generates and loads the next level */
	UFUNCTION(BlueprintCallable)
	void NextLevel();

	/* Tile Information */

	UPROPERTY(EditAnywhere, Category = "Tile")
	TSubclassOf<ATile> TileClass;

	// New Tiles

	UPROPERTY(EditAnywhere, Category = "Tile")
	TSubclassOf<ATile> StartTileClass;

	UPROPERTY(EditAnywhere, Category = "Tile")
	TSubclassOf<ATile> EnemyTileClass;

	UPROPERTY(EditAnywhere, Category = "Tile")
	TSubclassOf<ATile> GoalTileClass;

	UPROPERTY(EditAnywhere, Category = "Tile")
	int TileSize = 250;

	/* The Maximum number of attempts a composite can take to layout */
	UPROPERTY(EditAnywhere, Category = "LevelManager")
	int LayoutAttempts = 4;

	UPROPERTY(EditAnywhere, Category = "LevelManager")
	int CoarseFactor = 2;

	UPROPERTY(EditAnywhere, Category = "LevelManager")
	int FineFactor = 10;

	UPROPERTY(EditAnywhere, Category = "Room")
	int RoomSize = 9;

	UPROPERTY(EditAnywhere, Category = "LevelManager")
	int LevelNumMax = 9;

	/* Level Number corresponds to the index of the LevelManagers Levels Array - incremented upon level completion */
	UPROPERTY()
	int LevelNumber = 0;

private:

	/* Struct that holds the values that represent the player's player type model */
	UPROPERTY()
	FPlayerModel PlayerModel;

	/* Holds all Level Graphs and calls on GraphManager to edit them */
	UPROPERTY()
	ULevelManager* LevelManager;

	/* Contains all graph transformation functions  */
	UPROPERTY()
	UGraphManager* GraphManager;

	/* Contains all room generation functions */
	UPROPERTY()
	URoomManager* RoomManager;
};
