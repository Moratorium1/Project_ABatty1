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
	START,

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
	TArray<TArray<ETileType>> GenerateStartRoom(ULevelGraph* Level, UGraphNode* RoomKey);

	/* Creates a basic room */
	TArray<TArray<ETileType>> GenerateRoom(ULevelGraph* Level, UGraphNode* RoomKey);

	/*  */
	TArray<TArray<ETileType>> GenerateEdge(ULevelGraph* Level, UGraphNode* RoomKey);

};
