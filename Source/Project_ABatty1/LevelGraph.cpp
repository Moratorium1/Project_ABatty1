// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGraph.h"
#include "Graph.h"

void ULevelGraph::Initialise()
{
	OverallLevel = NewObject<UGraph>();
}