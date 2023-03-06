// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphEdge.h"
#include "GraphNode.h"

UGraphEdge::UGraphEdge() {}

/* Create an edge passing two nodes for the from and to Nodes */
UGraphEdge::UGraphEdge(UGraphNode* NodeTo, UGraphNode* NodeFrom)
{
	From = NodeFrom;

	To = NodeTo;
}

/* Returns From */
UGraphNode* UGraphEdge::GetFrom() { return From; }

/* Returns the Node the edge is to */
UGraphNode* UGraphEdge::GetTo() { return To; }

/* Returns the Type of the To Node */
ENodeType UGraphEdge::GetToType() { return To->GetType(); }

int UGraphEdge::GetUID()
{
	return UID;
}

void UGraphEdge::SetFrom(UGraphNode* Node)
{
	From = Node;
}

void UGraphEdge::SetTo(UGraphNode* Node)
{
	To = Node;
}

void UGraphEdge::SetUID(int& ID)
{
	UID = ID;
	ID++;
}
