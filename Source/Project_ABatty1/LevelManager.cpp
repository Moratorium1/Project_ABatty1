// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"
#include "LevelGraph.h"
#include "ProjectGameInstance.h"
#include "RoomManager.h"
#include "GraphManager.h"
#include "GraphNode.h"
#include "Tile.h"
#include "SpawnTile.h"
#include "EnemyBase.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/LevelStreamingDynamic.h"
#include "Engine/World.h"

void ULevelManager::Initialise()
{
	GameInstance = Cast<UProjectGameInstance>(GetGameInstance());

	GraphManager = GameInstance->GetSubsystem<UGraphManager>();
	GraphManager->Initialise();

	RoomManager = GameInstance->GetSubsystem<URoomManager>();
	RoomManager->Initialise();

	// Initialise the array with 10 elements of nullptr then replace each with its own LevelGraph
	Levels.Init(nullptr, 10);
}

void ULevelManager::GenerateLevel(const int& LevelNumber)
{
	/*  Destory all exisitng tiles
	*	Initialise the Level - Add the starting node
	*	Select a recipe
	*	Execute the recipe
	*	Set the adjacent nodes	- Each composite node needs to know its adjacent composite to be placed
	*	Extract the composites	- Short chain of nodes starting at a composite node
	*	Layout the composites	- Layout each composite walking through the grid placing each node of the composite
	*	If LayoutComposite returns false, Call GenerateLevel and start again
	*/

	// Destory all exisiting tiles 
	// Swap to ATile
	for (TObjectIterator<ATile> Itr; Itr; ++Itr) Itr->Destroy();
	for (TObjectIterator<AEnemyBase> Itr; Itr; ++Itr) Itr->Destroy();

	// Initialise the Level - Add the starting node
	ULevelGraph* LevelGraph = NewObject<ULevelGraph>();
	Levels[LevelNumber] = LevelGraph;
	Levels[LevelNumber]->Initialise();

	// Select and Execute the recipe on current level
	GraphManager->ExecuteRecipe(GraphManager->ChooseRecipe(), Levels[LevelNumber]);

	// Perform any injections add to the queue by the player
	GraphManager->ResolveInjectionQueue(Levels[LevelNumber]);

	// Set each composite nodes Adj Composite value
	GraphManager->SetAdjComposites(Levels[LevelNumber]);
	GraphManager->ExtractComposites(Levels[LevelNumber]);

	// Layout the composites - if a composite fails to layout recall this funciton
	if (!GraphManager->LayoutComposites(Levels[LevelNumber]))
		GenerateLevel(LevelNumber);

	// If the level has been sucessfully laid out clear the injection queue - otherwise queued will appear on every level
	GraphManager->ClearInjectionQueue();

	// Initialise the Coarse Grid - Grid introduces graph nodes of ENodeType::EDGE between nodes with edges
	GraphManager->InitialiseCoarseGrid(Levels[LevelNumber]);

	// Initialise the Fine Grid - scales up the Coarse grid allocating a FineFactor x FineFactor grid to each node
	GraphManager->InitialiseFineGrid(Levels[LevelNumber]);

	// For each Node of the coarse grid add a 2D array to the rooms TMap - The 2D array represents the room of the node
	RoomManager->ExtractRooms(Levels[LevelNumber]);

	RoomManager->GenerateRooms(Levels[LevelNumber]);

	//SpawnLevel(LevelNumber);

	// Spawns tiles into the world
	SpawnLevel(LevelNumber);
}

void ULevelManager::SpawnLevel(const int& LevelNumber)
{
	ULevelGraph* Level = Levels[LevelNumber];

	TArray<UGraphNode*> RoomKeys;
	Level->Rooms.GetKeys(RoomKeys);

	for (UGraphNode* RoomKey : RoomKeys)
	{
		TArray<TArray<ETileType>> Room = Level->Rooms[RoomKey];
		FIntPoint RoomPosition = FIntPoint(RoomKey->LayoutCoords.X * (Room.Num() * GameInstance->TileSize), RoomKey->LayoutCoords.Y * (Room.Num() * GameInstance->TileSize));

		for (int x = 0; x < Room.Num(); x++)
			for (int y = 0; y < Room.Num(); y++)
			{
				switch (Room[x][y])
				{
				case ETileType::START:
					GetWorld()->SpawnActor<ATile>(GameInstance->StartTileClass, FVector(RoomPosition.X + (x * GameInstance->TileSize), RoomPosition.Y + (y * GameInstance->TileSize), 0), FRotator::ZeroRotator);
					break;

				case ETileType::FLOOR:
					GetWorld()->SpawnActor<ATile>(GameInstance->TileClass, FVector(RoomPosition.X + (x * GameInstance->TileSize), RoomPosition.Y + (y * GameInstance->TileSize), 0), FRotator::ZeroRotator);
					break;

				case ETileType::ENEMY:
					GetWorld()->SpawnActor<ATile>(GameInstance->EnemyTileClass, FVector(RoomPosition.X + (x * GameInstance->TileSize), RoomPosition.Y + (y * GameInstance->TileSize), 0), FRotator::ZeroRotator);
					break;

				case ETileType::GOAL:
					GetWorld()->SpawnActor<ATile>(GameInstance->GoalTileClass, FVector(RoomPosition.X + (x * GameInstance->TileSize), RoomPosition.Y + (y * GameInstance->TileSize), -20), FRotator::ZeroRotator);
					break;

				}
			}
	}
}
