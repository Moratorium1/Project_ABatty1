// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphNode.h"
#include "GraphEdge.h"

void UGraphNode::SetType(const ENodeType NodeType)
{
	Type = NodeType;
}

ENodeType UGraphNode::GetType()
{
	return Type;
}

TArray<UGraphEdge*>& UGraphNode::GetEdges()
{
	return Edges;
}

bool UGraphNode::NodesMatch(UGraphNode* Node)
{
	if (Node != nullptr)
		return TypeMatch(Node) && EdgeMatch(Node);

	return false;
}

bool UGraphNode::TypeMatch(UGraphNode* Node)
{
	return Type == Node->GetType();
}

bool UGraphNode::EdgeMatch(UGraphNode* Node)
{
	/* For each edge of the passed node add false to the map */
	TMap<UGraphEdge*, bool> bMatched;
	for (UGraphEdge* Edge : Node->GetEdges())
		bMatched.Add(Edge, false);

	int Count = 0;	// A count of all the edges matched
	for (UGraphEdge* Target : Node->GetEdges())	// For each edge of the passed node
		for (auto& Edge : Edges)			// Compare it to each edge of the node
		{
			/* If the Edge is already matched check if the node the edges go to are the same type */
			if (!bMatched[Target] && Edge->GetToType() == Target->GetToType())
			{
				bMatched[Target] = true;		// Mark the edge as matched
				Count++;					// Increase the count of matched edges
				break;
			}
		}

	// Return true if a edge was matched to each of the passed nodes edges
	return Count >= Node->GetEdges().Num();
}
