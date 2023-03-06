// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Graph.h"
#include "GraphEdge.generated.h"

class UGraphNode;

UCLASS()
class PROJECT_ABATTY1_API UGraphEdge : public UObject
{
	GENERATED_BODY()

public:

	UGraphEdge();

	/* Create a new edge with a set to and from node */
	UGraphEdge(UGraphNode* NodeTo, UGraphNode* NodeFrom);

	/* Returns the Node the edge is from */
	UFUNCTION()
	UGraphNode* GetFrom();

	/* Returns the Node the edge is to */
	UFUNCTION()
	UGraphNode* GetTo();

	/* Returns the type of the to Node */
	UFUNCTION()
	ENodeType GetToType();

	/* Returns the Edges UID */
	UFUNCTION()
	int GetUID();

	/* Set the from Node */
	UFUNCTION()
	void SetFrom(UGraphNode* Node);

	/* Set the to Node */
	UFUNCTION()
	void SetTo(UGraphNode* Node);

	/* Set the UID */
	UFUNCTION()
	void SetUID(int& ID);

private:

	/* Unique ID used for the transformation of a graph - Corresponds with the edge of the right graph */
	UPROPERTY()
	int UID = -1;

	/* The Node the edge is from, the Node that holds the edge */
	UPROPERTY()
	UGraphNode* From = nullptr;

	/* The Node the edge is to */
	UPROPERTY()
	UGraphNode* To = nullptr;
};
