// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"
#include "LevelGraph.h"
#include "ProjectGameInstance.h"
#include "GraphManager.h"
#include "GraphNode.h"
#include "ATile.h"


void ULevelManager::Initialise()
{
	GameInstance = Cast<UProjectGameInstance>(GetGameInstance());

	GraphManager = GameInstance->GetSubsystem<UGraphManager>();
	GraphManager->Initialise();

	// Initialise the array with 10 elements of nullptr then replace each with its own LevelGraph
	Levels.Init(nullptr, 10);
	for (int i = 0; i < Levels.Num(); i++)
	{
		ULevelGraph* LevelGraph = NewObject<ULevelGraph>();
		Levels[i] = LevelGraph;
		Levels[i]->Initialise();
	}

	GenerateLevel(0);
}

void ULevelManager::GenerateLevel(const int& LevelNumber)
{
	/*	Initialise the Level - Place the first starting node
	*	Select a recipe
	*	Execute the recipe
	*	Set the adjacent nodes	- Each composite node needs to know its adjacent composite to be placed
	*	Extract the composites	- Short chain of nodes starting at a composite node
	*	Layout the composites	- Layout each composite walking through the grid placing each node of the composite
	*	If LayoutComposite returns false, Call GenerateLevel and start again
	*/

	/* S = Shop new start is HUB*/
	FString nodes = "Z";
	FString edges = "";

	Levels[LevelNumber]->OverallLevel->SetNodesString(nodes);
	Levels[LevelNumber]->OverallLevel->SetEdgesString(edges);
	GraphManager->ExecuteRule(TEXT("Start"), Levels[LevelNumber]);
	//GraphManager->ExecuteRule(TEXT("ResolveCycle"), Levels[LevelNumber]);
	GraphManager->ExecuteRule(TEXT("ResolveTree"), Levels[LevelNumber]);
	GraphManager->ExecuteRule(TEXT("InjectTree"), Levels[LevelNumber]);
	GraphManager->ExecuteRule(TEXT("ResolveTree"), Levels[LevelNumber]);

	//GraphManager->ExecuteRule(TEXT("InjectHub"), Levels[LevelNumber]);
	//GraphManager->ExecuteRule(TEXT("Hub"), Levels[LevelNumber]);
	GraphManager->ExecuteRule(TEXT("ResolveTree"), Levels[LevelNumber]);




	GraphManager->SetAdjComposites(Levels[LevelNumber]);
	GraphManager->ExtractComposites(Levels[LevelNumber]);
	GraphManager->LayoutComposites(Levels[LevelNumber]);

	SpawnLevel(LevelNumber);
}

void ULevelManager::SpawnLevel(const int& LevelNumber)
{
	TArray<TArray<UGraphNode*>> CurrentLevel = Levels[LevelNumber]->Layout;

	int TileSize = GameInstance->TileSize;

	for (int X = 0; X < CurrentLevel.Num() -1; X++)
		for (int Y = 0; Y < CurrentLevel.Num() - 1; Y++)
			if (CurrentLevel[X][Y] != nullptr)
			{
				ENodeType Type = CurrentLevel[X][Y]->GetType();

				switch (Type)
				{
				case ENodeType::START:
					GetWorld()->SpawnActor<AATile>(GameInstance->StartClass, FVector(X * TileSize, Y * TileSize, 0), FRotator::ZeroRotator);
					break;

				case ENodeType::start:
					GetWorld()->SpawnActor<AATile>(GameInstance->StartClass, FVector(X * TileSize, Y * TileSize, 25), FRotator::ZeroRotator);
					break;

				case ENodeType::hub:
					GetWorld()->SpawnActor<AATile>(GameInstance->HubClass, FVector(X * TileSize, Y * TileSize, 0), FRotator::ZeroRotator);
					break;

				case ENodeType::HUB:
					GetWorld()->SpawnActor<AATile>(GameInstance->HubClass, FVector(X * TileSize, Y * TileSize, 25), FRotator::ZeroRotator);
					break;

				case ENodeType::inject:
					GetWorld()->SpawnActor<AATile>(GameInstance->InjectClass, FVector(X * TileSize, Y * TileSize, 0), FRotator::ZeroRotator);
					break;

				case ENodeType::INJECT:
					GetWorld()->SpawnActor<AATile>(GameInstance->InjectClass, FVector(X * TileSize, Y * TileSize, 25), FRotator::ZeroRotator);
					break;

				case ENodeType::tree:
					GetWorld()->SpawnActor<AATile>(GameInstance->TreeClass, FVector(X * TileSize, Y * TileSize, 0), FRotator::ZeroRotator);
					break;

				// Cycles not implemented
				case ENodeType::cycle:
					GetWorld()->SpawnActor<AATile>(GameInstance->CycleClass, FVector(X * TileSize, Y * TileSize, 0), FRotator::ZeroRotator);
					break;

				default:
					GetWorld()->SpawnActor<AATile>(GameInstance->TileClass, FVector(X * TileSize, Y * TileSize, 0), FRotator::ZeroRotator);
				}
			}
}
