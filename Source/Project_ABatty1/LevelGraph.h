// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelGraph.generated.h"

class UGraph;
class UGraphNode;

UCLASS()
class PROJECT_ABATTY1_API ULevelGraph : public UObject
{
	GENERATED_BODY()
	
public:

	friend class UGraphRewriter;

	/* Initialise */
	UFUNCTION()
	void Initialise();

	/* Holds the entire level graph */
	UPROPERTY()
	UGraph* OverallLevel;

	/* Holds composite sections of the level graph - Used for mapping graph to 2D space */
	UPROPERTY()
	TMap<UGraphNode*, UGraph*> Composites;

	/* 2D Array of Nodes - Indexes represent the XY cooridnates of the nodes within the world */
	TArray<TArray<UGraphNode*>> Layout;

	/* An intermediate representation of the level adds between nodes */

	TArray<TArray<UGraphNode*>> CoarseGrid;

};
