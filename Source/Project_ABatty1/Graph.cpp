// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph.h"
#include "GraphNode.h"
#include "GraphEdge.h"
#include "Kismet/KismetStringLibrary.h"

void UGraph::SetNodes(const TArray<ENodeType> NodeTypes)
{
	/*
		Empty the nodes array
		for each element create a new node, set its type to the type of the element
		add the node to the Nodes array
	*/
	Nodes.Empty();

	for (ENodeType Type : NodeTypes)
	{
		auto Node = NewObject<UGraphNode>();
		Node->SetType(Type);

		Nodes.Add(Node);
	}
}

/* Set the Nodes of the graph by passing in a string */
void UGraph::SetNodesString(const FString& String)
{
	/* 
		Convert the string using the StringToENodeType function 
		Set the Nodes by passing in the resulting array of node types
	*/

	TArray<ENodeType> NodeTypes = StringToENodeType(String);
	SetNodes(NodeTypes);
}

/* Returns the Nodes array of the graph by reference */
TArray<UGraphNode*>& UGraph::GetNodes() { return Nodes; }

/* Returns the Edges array of the graph by reference */
TArray<UGraphEdge*>& UGraph::GetEdges() { return Edges; }

/* Create GraphEdges between the nodes of the Nodes array by passing in a Array of FIntPoint where X is index of the node the From and Y the index to */
void UGraph::SetEdges(const TArray<FIntPoint> EdgePoints)
{
	/*
		Loop through the passed array of FIntPoint
		For each create a new GraphEdge
		Set the GraphEdge From to the Node at the index of the Y
		Set the GraphEdge To to the Node at the index of the X
		Add the GraphEdge to the Edges array of the Graph
		Add the GraphEdge to the Edges of the From Node
	*/
	for (FIntPoint IntPoint : EdgePoints)
	{
		UGraphEdge* Edge = NewObject<UGraphEdge>();
		Edge->SetFrom(Nodes[IntPoint.X]);
		Edge->SetTo(Nodes[IntPoint.Y]);

		Edges.Add(Edge);
		Nodes[IntPoint.X]->GetEdges().Add(Edge);
	}
}

/* Set the Edges of the Nodes using a string */
void UGraph::SetEdgesString(const FString& String)
{
	/*
		Convert the passed string into a array of FIntPoint
		Use the resulting array to set the Edges
	*/
	TArray<FIntPoint> FIntPoints = StringToFIntPoint(String);
	SetEdges(FIntPoints);
}

/* Sets the Nodes and Edges of the Graph */
void UGraph::SetGraph(const TArray<ENodeType> NodeTypes, const TArray<FIntPoint> EdgePoints)
{
	/*
		Calls the two functions to set the Nodes and Edges 
		Nodes must be first as Edges are set using the indexes of the Nodes array
	*/

	SetNodes(NodeTypes);
	SetEdges(EdgePoints);
}

/* Takes a string and converts it to a Array<FIntPoint> that represents the Edges between Nodes */
TArray<FIntPoint> UGraph::StringToFIntPoint(const FString String)
{
	/*
		Take the chars in pairs convert them from strings to int values
		Create a FIntPoint using the values add them to the array
	*/

	TArray<FIntPoint> FIntPoints;
	for (int i = 0; i < String.Len(); i += 2)
	{
		int EdgeFrom = UKismetStringLibrary::Conv_StringToInt(String.Mid(i, 1));
		int EdgeTo = UKismetStringLibrary::Conv_StringToInt(String.Mid(i + 1, 1));

		FIntPoint NewEdge = FIntPoint(EdgeFrom, EdgeTo);

		FIntPoints.Add(NewEdge);
	}

	return FIntPoints;
}

/* Takes a string and converts it to a Array<ENodeType> to be used to set the Nodes */
TArray<ENodeType> UGraph::StringToENodeType(const FString String)
{
	TArray<ENodeType> NodeTypes;
	for (int i = 0; i < String.Len(); i++)
	{
		FString Type = String.Mid(i, 1);

		if (Type.Equals(TEXT("H")))
			NodeTypes.Add(ENodeType::HUB);
		else if (Type.Equals(TEXT("h")))
			NodeTypes.Add(ENodeType::hub);

		else if (Type.Equals(TEXT("C")))
			NodeTypes.Add(ENodeType::CYCLE);
		else if (Type.Equals(TEXT("c")))
			NodeTypes.Add(ENodeType::cycle);

		else if (Type.Equals(TEXT("T")))
			NodeTypes.Add(ENodeType::TREE);
		else if (Type.Equals(TEXT("t")))
			NodeTypes.Add(ENodeType::tree);

		else if (Type.Equals(TEXT("I")))
			NodeTypes.Add(ENodeType::INJECT);
		else if (Type.Equals(TEXT("i")))
			NodeTypes.Add(ENodeType::inject);

		else if (Type.Equals(TEXT("Z")))
			NodeTypes.Add(ENodeType::START);
		else if (Type.Equals(TEXT("z")))
			NodeTypes.Add(ENodeType::start);

		else if (Type.Equals(TEXT("N")))
			NodeTypes.Add(ENodeType::ROOM);
		else if (Type.Equals(TEXT("n")))
			NodeTypes.Add(ENodeType::room);

		else if (Type.Equals(TEXT("O")))
			NodeTypes.Add(ENodeType::KEY);
		else if (Type.Equals(TEXT("o")))
			NodeTypes.Add(ENodeType::key);

		else if (Type.Equals(TEXT("L")))
			NodeTypes.Add(ENodeType::LOCK);
		else if (Type.Equals(TEXT("l")))
			NodeTypes.Add(ENodeType::lock);

		else if (Type.Equals(TEXT("K")))
			NodeTypes.Add(ENodeType::KILLER);
		else if (Type.Equals(TEXT("k")))
			NodeTypes.Add(ENodeType::killer);

		else if (Type.Equals(TEXT("A")))
			NodeTypes.Add(ENodeType::ACHIEVER);
		else if (Type.Equals(TEXT("a")))
			NodeTypes.Add(ENodeType::achiever);

		else if (Type.Equals(TEXT("S")))
			NodeTypes.Add(ENodeType::SOCIALISER);
		else if (Type.Equals(TEXT("s")))
			NodeTypes.Add(ENodeType::socialiser);

		else if (Type.Equals(TEXT("E")))
			NodeTypes.Add(ENodeType::EXPLORER);
		else if (Type.Equals(TEXT("e")))
			NodeTypes.Add(ENodeType::explorer);

		else if (Type.Equals(TEXT("G")))
			NodeTypes.Add(ENodeType::GOAL);
		else if (Type.Equals(TEXT("g")))
			NodeTypes.Add(ENodeType::goal);

		else if (Type.Equals(TEXT("Q")))
			NodeTypes.Add(ENodeType::GOAL);
		else if (Type.Equals(TEXT("q")))
			NodeTypes.Add(ENodeType::goal);

		else
			NodeTypes.Add(ENodeType::INVALID);
	}
	return NodeTypes;
}
