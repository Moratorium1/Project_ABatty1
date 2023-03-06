// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Graph.h"
#include "GraphNode.generated.h"

class UGraphEdge;

UCLASS()
class PROJECT_ABATTY1_API UGraphNode : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void SetType(const ENodeType NodeType);

	UFUNCTION()
	ENodeType GetType();

	UFUNCTION()
	TArray<UGraphEdge*>& GetEdges();

	/* Returns true if the nodes edges and type match */
	UFUNCTION()
	bool NodesMatch(UGraphNode* Node);

	/* If the node is a composite node holds the adjacent composite node - used for combining the composites */
	UPROPERTY()
	UGraphNode* AdjComposite = nullptr;

	/* Indexes within the ULevelGraphs 2D Array for the Layout of the level - used only for composite nodes */
	FIntPoint LayoutCoords;

private:

	UPROPERTY()
	ENodeType Type = ENodeType::INVALID;

	UPROPERTY()
	TArray<UGraphEdge*> Edges;

	/* Returns true if the passed node is of the same ENodeType as the node */
	UFUNCTION()
	bool TypeMatch(UGraphNode* Node);

	/* Returns true if the node has an edge to the same types of nodes as the passed node - Still returns true of the node has additional edges */
	UFUNCTION()
	bool EdgeMatch(UGraphNode* Node);
};
