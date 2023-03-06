// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Graph.generated.h"

class UGraphNode;
class UGraphEdge;

/* Terminal and non-terminal node types*/
UENUM()
enum class ENodeType
{
	INVALID,
	//Non-Terminal
	//High Level
	HUB,
	CYCLE,
	TREE,
	INJECT,

	//Low Level
	START,
	ROOM,
	KEY,
	LOCK,
	KILLER,
	ACHIEVER,
	SOCIALISER,
	EXPLORER,
	GOAL,

	//Terminal
	//High Level
	hub,
	cycle,
	tree,
	inject,

	//Low Level
	start,
	room,
	key,
	lock,
	killer,
	achiever,
	socialiser,
	explorer,
	goal,

	MAX
};
UCLASS()
class PROJECT_ABATTY1_API UGraph : public UObject
{
	GENERATED_BODY()
	
public:

	friend class ULevelGraph;
	friend class UGraphManager;

	/* Set nodes via ENodeType Array */
	UFUNCTION()
	void SetNodes(const TArray<ENodeType> NodeTypes);

	/* Set nodes via a string  */
	UFUNCTION()
	void SetNodesString(const FString& String);

	UFUNCTION()
	TArray<UGraphNode*>& GetNodes();

	UFUNCTION()
	void SetEdges(const TArray<FIntPoint> EdgePoints);

	UFUNCTION()
	void SetEdgesString(const FString& String);

	UFUNCTION()
	void SetGraph(const TArray<ENodeType> NodeTypes, const TArray<FIntPoint> EdgePoints);

private:

	UPROPERTY()
	TArray<UGraphNode*> Nodes;

	UPROPERTY()
	TArray<UGraphEdge*> Edges;

	/* Converts a string to an array of FInPoint to then be parsed into GraphEdges between the Nodes*/
	UFUNCTION()
	TArray<FIntPoint> StringToFIntPoint(const FString String);

	/* Converts a string into an array of Node Types that is then parsed into Nodes added to the Node array */
	UFUNCTION()
	TArray<ENodeType> StringToENodeType(const FString String);
};
