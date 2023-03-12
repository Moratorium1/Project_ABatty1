// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RoomManager.generated.h"

class UProjectGameInstance;
class ULevelGraph;
class UGraphNode;

/* Terminal and non-terminal node types*/
UENUM()
enum class ETileType
{
	INVALID,

	EMPTY,
	FLOOR,
	ENEMY,
	BOSS,
	START,
	GOAL,
	LOCK,
	KEY,

	KILLER,
	ACHIEVER,
	SOCIALISER,
	EXPLORER,

	MAX
};

UCLASS()
class PROJECT_ABATTY1_API URoomManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	friend class ULevelManager;

public:

	UFUNCTION()
	void Initialise();

private:

	/* Reference to the GameInstance required to access the player type model */
	UPROPERTY()
	UProjectGameInstance* GameInstance = nullptr;

	UPROPERTY()
	int RoomSize = 8;

	/* Each node represents a room, for each node add a 2D array of its to the Rooms TMap */
	UFUNCTION()
	void ExtractRooms(ULevelGraph* Level);

	/* */
	UFUNCTION()
	void GenerateRooms(ULevelGraph* Level);

	/* Creates a basic room */
	TArray<TArray<ETileType>> GenerateBaseRoom(ULevelGraph* Level, UGraphNode* RoomKey);

	/* Creates a basic room and places enemies */
	TArray<TArray<ETileType>> GenerateRoom(ULevelGraph* Level, UGraphNode* RoomKey);

	/* Creates a basic room and places Boss */
	TArray<TArray<ETileType>> GenerateBossRoom(ULevelGraph* Level, UGraphNode* RoomKey);

	/* Creates a basic room and places a Start tile in the centre - start tile upon spawning moves the player to it*/
	TArray<TArray<ETileType>> GenerateStartRoom(ULevelGraph* Level, UGraphNode* RoomKey);

	/* Creates a basic room and places a Key tile in the centre*/
	TArray<TArray<ETileType>> GenerateKeyRoom(ULevelGraph* Level, UGraphNode* RoomKey);

	/* Creates a basic room and places a Key tile in the centre*/
	TArray<TArray<ETileType>> GeneratePlayerTypeRoom(ULevelGraph* Level, UGraphNode* RoomKey);

	/* Create a thin central strip of floors in the direction between the two nodes */
	TArray<TArray<ETileType>> GenerateEdge(ULevelGraph* Level, UGraphNode* RoomKey);

	/* Create a thin central strip of floors in the direction between the two nodes */
	TArray<TArray<ETileType>> GenerateLockedEdge(ULevelGraph* Level, UGraphNode* RoomKey);

	/* Create a basic room and places a Goal tile in the centre - goal tile creates overlap volume above it that loads the next level */
	TArray<TArray<ETileType>> GenerateGoalRoom(ULevelGraph* Level, UGraphNode* RoomKey);

};
