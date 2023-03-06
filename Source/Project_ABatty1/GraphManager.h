// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Graph.h"
#include "GraphManager.generated.h"

class UProjectGameInstance;
class FXmlNode;
class ULevelGraph;
class UGraph;
class URecipe;
class URule;

UENUM()
enum EPlayerType
{
	KILLER,
	ACHIEVER,
	SOCAILISER,
	EXPLORER
};

UCLASS()
class PROJECT_ABATTY1_API UGraphManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	friend class ULevelManager;

	UFUNCTION()
	void Initialise();

private:

	/* Reference to the GameInstance required to access the player type model */
	UPROPERTY()
	UProjectGameInstance* GameInstance = nullptr;

	/* Graph Rewrite Functions */

	/* Grammar Rule Execution Functions */

	/* */
	UFUNCTION()
	void ExecuteRecipe(const FString& RecipeName, ULevelGraph* Level);

	UFUNCTION()
	void ExecuteRule(const FString& RuleName, ULevelGraph* Level);

	/* Given an array of FStrings that are the names of subgraphs, return a TArray of these subgraph weighted so that there is a greater number of  */
	UFUNCTION()
	UGraph* ChooseRight(TArray<FString> SubGraphNames);

	/* Graph Search Functions */

	/* Finds a subgrpah within a graph using a BFS approach - Retruns all matching subgraphs found in FoundGraphs array  */
	UFUNCTION()
	bool FindSubgraph(ULevelGraph* Level, UGraph* Subgraph, TArray<UGraph*>& FoundGraphs);

	/* Performs a BFS starting from a given node attempting to find the target subgraph, constructing the subgraph and marking the found edges */
	UFUNCTION()
	void BreadthFirstSubgraphSearch(UGraphNode* Node, TMap<UGraphNode*, bool>& Visited, UGraph* Subgraph, UGraph* Target);

	/* Graph Transformation Functions */

	/* Transform a subgraph, Left, into subgraph Right */
	void TransformSubgraph(ULevelGraph* Level, UGraph& Subgraph, const UGraph* Left, const UGraph* Right);

	/* Remove a given edge from the graph, subgraph and the node */
	void RemoveEdge(UGraph& Graph, UGraph& Subgraph, UGraphEdge* Edge);

	/* Create a new edge adding it to the graph, subgraph and to the from node */
	void AddEdge(UGraph& Graph, UGraph& Subgraph, const int FromIndex, const int ToIndex);

	/* Insert a new node of the correct type into the subgraph at the correct index and add the node to the graph */
	void AddNode(UGraph& Graph, UGraph& Subgraph, const ENodeType NodeType, const int Index);

	/* Create a new node of a given type */
	UGraphNode* CreateNodeOfType(const ENodeType NodeType);


	/* Graph LayoutFunctions */

	/* Loop through the levels Composite array laying out each one */
	UFUNCTION()
	bool LayoutComposites(ULevelGraph* Level);

	/*  */
	bool LayoutTree(UGraph* Tree, TArray<TArray<UGraphNode*>>& Grid, int AdjX, int AdjY);

	bool LayoutHub(UGraph* Hub, TArray<TArray<UGraphNode*>>& Grid, int AdjX, int AdjY);


	UFUNCTION()
	TMap<FIntPoint, bool> GetDirectionMap();

	/*  */
	FIntPoint FindAdjUnoccupiedCell(const TArray<TArray<UGraphNode*>> Grid, int X, int Y);

	FIntPoint FindAdjUnoccupiedCell(const TArray<TArray<UGraphNode*>> Grid, int X, int Y, FIntPoint CurrentDir);

	/* */
	bool LayoutCycle(UGraph* Cycle, TArray<TArray<UGraphNode*>>& Grid, int AdjX, int AdjY);

	FIntPoint GetOpposingDirection(const FIntPoint& Direction);

	/* Sets the adjacent composite for every composite node except the hub */
	void SetAdjComposites(ULevelGraph* Level);

	/* Splits the graph into its Tree, Cycle and Hub composites */
	UFUNCTION()
	void ExtractComposites(ULevelGraph* Level);

	/* DFS algorithm that collects the nodes of the composite */
	UFUNCTION()
	void DepthFirstCompositeSearch(UGraphNode* Node, TArray<UGraphNode*>& Visited, UGraph* Composite);

	/* XML Functions */

	UFUNCTION()
	void LoadGraphGrammar(const FString& GrammarFile);

	///* Holds all the subgraph that make up grammar rules */
	UPROPERTY()
	TMap<FString, UGraph*> Subgraphs;

	/* Parse subgraphs detailed in xml into Subgraph TMap */
	void ParseGraphs(const FXmlNode* Root);

	/* Holds the grammar rules */
	UPROPERTY()
	TMap<FString, URule*> Rules;

	/* Parse rules detailed in xml into Rules TMap */
	void ParseRules(const FXmlNode* Root);

	/* Holds lists of rules - Recipes */
	UPROPERTY()
	TMap<FString, URecipe*> Recipes;

	/* Parse recipe detailed in xml into Recipes TMap */
	void ParseRecipes(const FXmlNode* Root);
};
