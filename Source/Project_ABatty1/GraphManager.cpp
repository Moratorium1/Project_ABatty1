// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphManager.h"
#include "ProjectGameInstance.h"
#include "Rule.h"
#include "Recipe.h"
#include "Graph.h"
#include "LevelGraph.h"
#include "GraphNode.h"
#include "GraphEdge.h"
#include "Kismet/KismetArrayLibrary.h"

#include "XmlFile.h"
#include <Runtime/XmlParser/Public/XmlFile.h>

void UGraphManager::Initialise()
{
    LoadGraphGrammar(TEXT("GrammarFile.xml"));

    GameInstance = Cast<UProjectGameInstance>(GetGameInstance());
}

void UGraphManager::ExecuteRecipe(const FString& RecipeName, ULevelGraph* Level)
{
    /* Execute each rule of the given recipe */

    auto Recipe = Recipes[RecipeName];
    if (!Recipe) return;

    for (auto rule : Recipe->GetRules())
        ExecuteRule(rule, Level);
}

/* Execute the specified rule, finding the Left subgraph within the passed level Graph and transforming it to a choosen right subgraph */
void UGraphManager::ExecuteRule(const FString& RuleName, ULevelGraph* Level)
{
    /* Execute the specified rule, finding the Left subgraph within the passed level Graph and transforming it to a choosen right subgraph
    *  If the Rule Mode is normal find all matching left and select one at random to transform
    *  if the Rule Mode is cellular find all matching left and transform all matching left
    */

    URule* Rule = Rules[RuleName];
    if (Rule)
    {
        UGraph* Left = Subgraphs[Rule->GetLeft()];

        TArray<UGraph*> Found;
        if (FindSubgraph(Level, Left, Found))
        {
            /* Choose a Subgraph for the right subgraph - May be chosen based on PlayerType or Random based on num of elements  */
            UGraph* Right = ChooseRight(Rule->GetRight());

            if (Rule->GetMode() == "NORMAL")
                TransformSubgraph(Level, *Found[FMath::RandRange(0, Found.Num() - 1)], Left, Right);
            else if (Rule->GetMode() == "CELLULAR")
                for (auto& Graph : Found)
                {
                    Right = ChooseRight(Rule->GetRight());
                    TransformSubgraph(Level, *Graph, Left, Right);
                }
        }
    }
}

/* Chooses a UGraph from a TArray of Subgraph names - Chosen based on PlayerType values or random based on num of elements */
UGraph* UGraphManager::ChooseRight(TArray<FString> SubGraphNames)
{
    /* If there is only a single element to the SubGraphNames then there is only a single subgraph to choose from
    *  If there is four elements then there is a element for each PlayerType and a weighted choice should be made
    *  If there is any other value of elements a random choice should be made
    */
    FRandomStream RandStream;
    RandStream.GenerateNewSeed();

    FString ChosenRight;
    int RandomIndex;

    if (SubGraphNames.Num() == 1) ChosenRight = SubGraphNames[0];
    else if (SubGraphNames.Num() == 4)
    {
        TArray<FString> WeightedRight;
        FPlayerModel PlayerModel = GameInstance->GetPlayerModel();

        for (int i = 0; i < PlayerModel.Killer; i++)        WeightedRight.Add(SubGraphNames[EPlayerType::KILLER]);
        for (int i = 0; i < PlayerModel.Achiever; i++)      WeightedRight.Add(SubGraphNames[EPlayerType::ACHIEVER]);
        for (int i = 0; i < PlayerModel.Socialiser; i++)    WeightedRight.Add(SubGraphNames[EPlayerType::SOCAILISER]);
        for (int i = 0; i < PlayerModel.Explorer; i++)      WeightedRight.Add(SubGraphNames[EPlayerType::EXPLORER]);

        RandomIndex = FMath::RandRange(0, WeightedRight.Num() - 1);
        ChosenRight = WeightedRight[RandomIndex];
    }
    else
    {
        RandomIndex = FMath::RandRange(0, SubGraphNames.Num() - 1);
        ChosenRight = SubGraphNames[RandomIndex];
    }

    return Subgraphs[ChosenRight];
}

/* Chooses a URecipe */
FString UGraphManager::ChooseRecipe()
{
    TArray<FString> RecipeNames; 
    Recipes.GetKeys(RecipeNames);

    // Generate a random index within the bounds of the array
    int32 RandIndex = FMath::RandRange(0, Recipes.Num() - 1);

    // Return the Recipe name at the random index
    return RecipeNames[RandIndex];
}

/* SUBGRAPH SEARCH FUNCTIONS START */

/* The TMap for Visited may not be necessary any more? - Findsubgraph loops through each node once regardless */
/* Find all matching subgrpahs within a Level - Output a TArray of the matching subgraphs */
bool UGraphManager::FindSubgraph(ULevelGraph* Level, UGraph* Subgraph, TArray<UGraph*>& FoundGraphs)
{
    UGraph* Graph = Level->OverallLevel;

    /* Create a map to track which nodes have been visited */
    TMap<UGraphNode*, bool> Visited;
    for (auto Node : Graph->Nodes)
        Visited.Add(Node, false);

    /* Loop through all nodes of the graph checking if the node could be the start of subgraph */
    for (auto Node : Graph->GetNodes())
    {   // If the Node has not been visited and is a match for the start of the subgraph
        if (!Visited[Node] && Node->NodesMatch(Subgraph->Nodes[0]))
        {   // Create a new graph to hold the matching nodes
            UGraph* FoundSubgraph = NewObject<UGraph>();

            // Perform a breadth first search from the Node attempting to find the rest of the subgraph 
            BreadthFirstSubgraphSearch(Node, Visited, FoundSubgraph, Subgraph);

            // If the FoundSubgraph has all the Nodes of the Target Subgraph add it to the FoundGraphs
            if (FoundSubgraph->Nodes.Num() == Subgraph->Nodes.Num())
                FoundGraphs.Add(FoundSubgraph);
        }
    }

    // returns true if Found Graphs is not empty and False if it is empty
    return !FoundGraphs.IsEmpty();
}

/* Performs a BFS starting from a given node attempting to find the target subgraph, constructing the subgraph and marking the found edges */
void UGraphManager::BreadthFirstSubgraphSearch(UGraphNode* Node, TMap<UGraphNode*, bool>& Visited, UGraph* Subgraph, UGraph* Target)
{
    /* Queue to hold nodes to be visited */
    TQueue<UGraphNode*> Q;
    int EdgeID = 0;

    /* Add the starting node to the queue and mark it as visited */
    Q.Enqueue(Node);
    Visited[Node] = true;

    /* Add the starting node to the subgraph */
    Subgraph->Nodes.Add(Node);

    /* if the Q is not empty */
    while (!Q.IsEmpty())
    {
        /* Dequeue the next node */
        UGraphNode* CurrentNode;
        Q.Dequeue(CurrentNode);

        /* For each edge of the current node */
        for (UGraphEdge* Edge : CurrentNode->GetEdges())
        {
            UGraphNode* To = Edge->GetTo();
            int TargetNodeIndex = Subgraph->Nodes.Num() - 1;

            /* if the To node of the edge has not been visited and matches the target node */
            if (Subgraph->Nodes.Num() < Target->Nodes.Num() && CurrentNode->NodesMatch(Target->Nodes[TargetNodeIndex]))
            {
                /* Add the To node to the Q and mark it as visited */
                Q.Enqueue(To);
                Visited[To] = true;

                /* Add the To node to the subgraph along with the edge to it - Set the UID of the Edge*/
                Subgraph->Nodes.Add(To);
                Subgraph->Edges.Add(Edge);
                Edge->SetUID(EdgeID);
            }
        }
    }
}

/* SUBGRAPH SEARCH FUNCTIONS END */

/* TRANSFORM FUNCTIONS START */

void UGraphManager::TransformSubgraph(ULevelGraph* Level, UGraph& Subgraph, const UGraph* Left, const UGraph* Right)
{
    /*  Changes are carried out on graph and subgraph at the same time
        both are required as the subgraph holds the nodes in the specific order required
        whereas new nodes can't be added to the graph in the specific order because it would depend
        on nodes outside of the subgraph 
    */

    UGraph* Graph = Level->OverallLevel;

    /* Delete all edges marked with UIDs */
    for (int i = Subgraph.Edges.Num() - 1; i >= 0; i--)
    {
        UGraphEdge* Edge = Subgraph.Edges[i];
        if (Edge->GetUID() != -1 || Subgraph.Nodes.Contains(Edge->GetTo())) RemoveEdge(*Graph, Subgraph, Edge);
    }

    /* Change corresponding Nodes to required type or create new nodes of the correct type */
    for (int i = 0; i < Right->Nodes.Num(); i++)
    {
        ENodeType Type = Right->Nodes[i]->GetType();

        if (i < Left->Nodes.Num())  Subgraph.Nodes[i]->SetType(Type);
        else AddNode(*Graph, Subgraph, Type, i);
    }

    /* for each Edge in the right graph create new edge that goes to and from the same respective indexes within the Right and Subgraph graphs */
    for (auto Edge : Right->Edges)
    {
        int FromIndex;
        Right->Nodes.Find(Edge->GetFrom(), FromIndex);

        int ToIndex;
        Right->Nodes.Find(Edge->GetTo(), ToIndex);

        AddEdge(*Graph, Subgraph, FromIndex, ToIndex);
    }
}

/* Removes Edge from the graph subgraph and the nodes edges */
void UGraphManager::RemoveEdge(UGraph& Graph, UGraph& Subgraph, UGraphEdge* Edge)
{
    Graph.Edges.Remove(Edge);
    Subgraph.Edges.Remove(Edge);
    Edge->GetFrom()->GetEdges().Remove(Edge);
}

/* Adds Edge to graph, subgraph and the FromNodes edges */
void UGraphManager::AddEdge(UGraph& Graph, UGraph& Subgraph, const int FromIndex, const int ToIndex)
{
    UGraphEdge* Edge = NewObject<UGraphEdge>();

    Edge->SetFrom(Subgraph.Nodes[FromIndex]);
    Edge->SetTo(Subgraph.Nodes[ToIndex]);

    Subgraph.Nodes[FromIndex]->GetEdges().Add(Edge);
    Graph.Edges.Add(Edge);
}

/* Creates a new node of the correct NodeType and Adds it to the Subgraph and Graph */
void UGraphManager::AddNode(UGraph& Graph, UGraph& Subgraph, const ENodeType NodeType, const int Index)
{
    UGraphNode* Node = CreateNodeOfType(NodeType);

    /* Add the node to the subgraph at the correct index */
    Subgraph.Nodes.Insert(Node, Index);

    /*  Add the node to the graph */
    Graph.Nodes.Add(Node);
}

/* Creates and returns a Node of a specified type */
UGraphNode* UGraphManager::CreateNodeOfType(const ENodeType NodeType)
{
    UGraphNode* Node = NewObject<UGraphNode>();
    Node->SetType(NodeType);

    return Node;
}

/* Executes all the injected rules */
void UGraphManager::ResolveInjectionQueue(ULevelGraph* Level)
{
    for (FString RuleQueued : GameInstance->InjectionQueue)
        ExecuteRule(RuleQueued, Level);
}

/* Empties the injection queue */
void UGraphManager::ClearInjectionQueue()
{
    GameInstance->InjectionQueue.Empty();
}

/* TRANSFORM FUNCTIONS END */

/* GRAPH LAYOUT FUNCTIONS START */

bool UGraphManager::LayoutComposites(ULevelGraph* Level)
{
    int Width = 16;
    int Height = 16;
    int X = Width / 2;
    int Y = Height / 2;

    TArray<TArray<UGraphNode*>> CombinedLevel;
    CombinedLevel.SetNum(Width);
    for (int x = 0; x < Width; x++)
    {
        CombinedLevel[x].SetNum(Height);
    }

    // Place the hub in the centre
    CombinedLevel[X][Y] = Level->OverallLevel->Nodes[0];
    CombinedLevel[X][Y]->LayoutCoords = FIntPoint(X, Y);

    for (auto Composite : Level->Composites)
    {
        FIntPoint Coords;
        UGraphNode* AdjComposite = Composite.Key->AdjComposite;

        // The first hub node will have no AdjComposite set so skip it
        if (AdjComposite == nullptr) continue;

        Coords = AdjComposite->LayoutCoords;

        bool success = false;
        for (int attempts = 0; attempts < GameInstance->LayoutAttempts; attempts++)
        {
            success = false;

            if (Composite.Key->GetType() == ENodeType::tree)
                success = LayoutTree(Composite.Value, CombinedLevel, Coords.X, Coords.Y);
            else if (Composite.Key->GetType() == ENodeType::cycle)
                success = LayoutCycle(Composite.Value, CombinedLevel, Coords.X, Coords.Y);
            else if (Composite.Key->GetType() == ENodeType::hub)
                success = LayoutHub(Composite.Value, CombinedLevel, Coords.X, Coords.Y);
            else if (Composite.Key->GetType() == ENodeType::start)
                success = true;

            if (success) break;
        } 

        // If all attempts fail to layout the composite return false
        if (!success) return false;
    }
    Level->Layout = CombinedLevel;

    return true;
}

bool UGraphManager::LayoutTree(UGraph* Tree, TArray<TArray<UGraphNode*>>& Grid, int AdjX, int AdjY)
{
    /* Initialise a TMap to track the positions around the AdjComp Node that the first node of the tree has been placed
    *
    */
    TArray<TArray<UGraphNode*>> GridCopy = Grid;

    UGraphNode* Current = Tree->Nodes[0];
    FIntPoint CurrentDir = FIntPoint(0, 0);

    int NewX = AdjX;
    int NewY = AdjY;

    while (Current != nullptr)
    {
        CurrentDir = FindAdjUnoccupiedCell(Grid, NewX, NewY, CurrentDir);

        if (CurrentDir != FIntPoint::ZeroValue)
        {
            // Update position and add to backtrack points
            NewX += CurrentDir.X;
            NewY += CurrentDir.Y;

            Grid[NewX][NewY] = Current;
            Current->LayoutCoords = FIntPoint(NewX, NewY);

            // Move to next node in tree
            if (Current->GetEdges().Num() > 0)
            {
                for (auto Edge : Current->GetEdges())
                {
                    UGraphNode* Node = Edge->GetTo();
                    if (Tree->Nodes.Contains(Node))
                    {
                        Current = Node;
                        break;
                    }
                    else return true;
                }
            }
            else return true; // Tree successfully placed
        }
        else break;
    }

    // Revert Grid
    Grid = GridCopy;
    return false;
}

bool UGraphManager::LayoutHub(UGraph* Hub, TArray<TArray<UGraphNode*>>& Grid, int AdjX, int AdjY)
{
    FIntPoint CurrentDir = FIntPoint(0, 0);
    CurrentDir = FindAdjUnoccupiedCell(Grid, AdjX, AdjY, CurrentDir);

    if (CurrentDir != FIntPoint::ZeroValue)
    {
        int NewX = AdjX + CurrentDir.X;
        int NewY = AdjY + CurrentDir.Y;
        Grid[NewX][NewY] = Hub->Nodes[0];
        Grid[NewX][NewY]->LayoutCoords = FIntPoint(NewX, NewY);
        return true;
    }
    else return false;
}

/* Returns the direction to a unoccupied cell within the grid adj to the coordinates given - A Current direction can be passed in to weight the function to maintain the direction */
FIntPoint UGraphManager::FindAdjUnoccupiedCell(const TArray<TArray<UGraphNode*>>& Grid, int X, int Y, FIntPoint CurrentDir)
{
    FRandomStream RandStream;
    RandStream.GenerateNewSeed();

    const int NumDirections = 6;
    int XDirections[NumDirections] = { 0, 1, 0, -1, CurrentDir.X, CurrentDir.X };
    int YDirections[NumDirections] = { 1, 0, -1, 0, CurrentDir.Y, CurrentDir.Y };

    TArray<int> DirectionIndex;
    for (int i = 0; i < NumDirections; i++)
    {
        DirectionIndex.Add(i);
    }

    for (int i = DirectionIndex.Num() - 1; i > 0; i--)
    {
        int j = FMath::RandRange(0, i);
        DirectionIndex.Swap(i, j);
    }

    // Check each direction for an unoccupied cell
    FIntPoint NewDir = FIntPoint(0, 0);
    for (int Direction : DirectionIndex)
    {
        int NewX = X + XDirections[Direction];
        int NewY = Y + YDirections[Direction];
        if (NewX >= 0 && NewX < Grid.Num() && NewY >= 0 && NewY < Grid[0].Num() && Grid[NewX][NewY] == nullptr)
        {
            NewDir = FIntPoint(XDirections[Direction], YDirections[Direction]);
            X = NewX;
            Y = NewY;
            break;
        }
    }

    return NewDir;
}

void UGraphManager::InitialiseCoarseGrid(ULevelGraph* Level)
{
    int CoarseFactor = GameInstance->CoarseFactor;

    TArray<TArray<UGraphNode*>> Layout = Level->Layout;
    TArray<TArray<UGraphNode*>> CoarseGrid = Level->CoarseGrid;

    int GridSize = Layout.Num() * CoarseFactor;

    CoarseGrid.SetNumZeroed(GridSize);
    for (int x = 0; x < GridSize; x++)
        CoarseGrid[x].SetNumZeroed(GridSize);

    for (int x = 0; x < GridSize; x++)
        for (int y = 0; y < GridSize; y++)
        {
            if (x % CoarseFactor != 0 && y % CoarseFactor != 0)
            {
                CoarseGrid[x][y] = Layout[x / CoarseFactor][y / CoarseFactor];

                if (CoarseGrid[x][y])
                    CoarseGrid[x][y]->LayoutCoords = FIntPoint(x, y);
            }
        }
    Level->CoarseGrid = CoarseGrid;
    SetBetweenNodes(Level);
}

void UGraphManager::SetBetweenNodes(ULevelGraph* Level)
{
    /* Loop through the nodes of a level
    *  if the node has an edge
    *  get the To node
    *  compare the LayoutCoords of the From and To to determine the direcion of the node in the layout
    *  place a UGraphNode of ENodeType::EDGE at the From LayoutCoords + DirectionToTo/2 in the coarseGrid
    *  set the edge nodes EdgeDirection for use in laying out the level  
    * 
    *  This should result in the CoarseGrid having an edge node between all connected nodes
    */

    TArray<TArray<UGraphNode*>> Layout = Level->Layout;
    TArray<TArray<UGraphNode*>> CoarseGrid = Level->CoarseGrid;

    for (UGraphNode* Node : Level->OverallLevel->Nodes)
    {
        if (Node->GetEdges().Num())
            for (UGraphEdge* Edge : Node->GetEdges())
            {
                FIntPoint FromNodeCoords = Node->LayoutCoords;
                FIntPoint ToNodeCoords = Edge->GetTo()->LayoutCoords;
                FIntPoint DirectionToTo = FIntPoint(ToNodeCoords.X - FromNodeCoords.X, ToNodeCoords.Y - FromNodeCoords.Y);

                UGraphNode* NewEdge;
                if (Node->GetType() != ENodeType::LOCK)
                    NewEdge = CreateNodeOfType(ENodeType::EDGE);
                else
                    NewEdge = CreateNodeOfType(ENodeType::LOCKED_EDGE);

                NewEdge->LayoutCoords = FIntPoint(FromNodeCoords.X + (DirectionToTo.X/2), FromNodeCoords.Y + (DirectionToTo.Y/2));
                NewEdge->EdgeDirection = FIntPoint(DirectionToTo.X / 2, DirectionToTo.Y / 2);

                CoarseGrid[FromNodeCoords.X + (DirectionToTo.X/2)][FromNodeCoords.Y + (DirectionToTo.Y/2)] = NewEdge;
            }
    }
    Level->CoarseGrid = CoarseGrid;
}

void UGraphManager::InitialiseFineGrid(ULevelGraph* Level)
{
    TArray<TArray<UGraphNode*>> CoarseGrid = Level->CoarseGrid;
    TArray<TArray<UGraphNode*>> FineGrid = Level->FineGrid;

    int FineFactor = GameInstance->FineFactor;
    int GridSize = CoarseGrid.Num() * FineFactor;

    FineGrid.SetNumZeroed(GridSize);
    for (int x = 0; x < GridSize; x++)
        FineGrid[x].SetNumZeroed(GridSize);

    for (int x = 0; x < GridSize; x++)
        for (int y = 0; y < GridSize; y++)
        {
                FineGrid[x][y] = CoarseGrid[FMath::Floor(x / FineFactor)][FMath::Floor(y / FineFactor)];
        }
    Level->FineGrid = FineGrid;
}

bool UGraphManager::LayoutCycle(UGraph* Cycle, TArray<TArray<UGraphNode*>>& Grid, int AdjX, int AdjY)
{
    /* In order to layout a cycle a unoccupied cell adj to the adjComp of the first cycle node needs to be selected
    *  The aim of the function is to select two directions away from the adjcomp node so that a square within the grid
    *  can be checked 
    *    if the square is empty then the cycle can be layed out in that area
    *    if the square is occupied then try another direction
    *       if this is also occupied return false so that LayoutCycle can call the function again and move the cycle node
    *  
    *   When selecting the directions for the square to check the direction away from the adjcomp node must be selected
    *   e.g. if the cycle node is placed north of the adjcomp node, the direction to it is south and north has to be selected
    *   otherwise the square will return occupied as it contains the adjcomp node
    *   continuing this example with north selected either east or west must be the second direction
    *   if the square is returned as occupied then the other possible second direction should be checked 
    * 
    */

    FRandomStream RandStream;
    RandStream.GenerateNewSeed();

    UGraphNode* Current = Cycle->Nodes[0];

    // Place the Cycle node onto the grid then store the direction from the AdjComp node to the cycleNode as OpposingDirection
    FIntPoint CurrentDirection = FIntPoint::ZeroValue;
    FIntPoint OpposingDirection = FindAdjUnoccupiedCell(Grid, AdjX, AdjY, CurrentDirection);
    // Then get the Direction opposing the direction to the Cycle node as the DirectionToAdjacent
    FIntPoint DirectionToAdjacent = GetOpposingDirection(OpposingDirection);

    int CycleX = AdjX + OpposingDirection.X;
    int CycleY = AdjY + OpposingDirection.Y;

    Grid[CycleX][CycleY] = Current;

    // Create an array of directions
    const int NumDirections = 4;
    int XDirections[NumDirections] = { 0, 1, 0, -1 };
    int YDirections[NumDirections] = { 1, 0, -1, 0 };

    TArray<int> Directions;
    for (int i = 0; i < NumDirections; i++) Directions.Add(i);

    // Remove the OpposingDirection and DirectiontoAdj from Direction array
    for (int Direction : Directions)
    {
        int DirX = XDirections[Direction];
        int DirY = YDirections[Direction];

        if (DirX == OpposingDirection.X && DirY == OpposingDirection.Y)
        {
            Directions.Remove(Direction);
            break;
        }
    }
    Directions.Shrink();

    for (int Direction : Directions)
    {
        int DirX = XDirections[Direction];
        int DirY = YDirections[Direction];

        if (DirX == DirectionToAdjacent.X && DirY == DirectionToAdjacent.Y)
        {
            Directions.Remove(Direction);
            break;
        }
    }
    Directions.Shrink();

    // Either remaining direction can be selected, and if the first selected fails select the other
    // Select a remaining direction at random and remove the element at the selected index
    const int32 RandomIndex = FMath::RandRange(0, Directions.Num() - 1);
    FIntPoint SelectedDirection = FIntPoint(XDirections[RandomIndex], YDirections[RandomIndex]);
    Directions.RemoveAt(RandomIndex);

    // Combine the two selected directions into a single fIntPoint - from each direction select the value that is not 0
    FIntPoint CombinedDirection = FIntPoint::ZeroValue;
    CombinedDirection.X = OpposingDirection.X == 0 ? SelectedDirection.X : OpposingDirection.X;
    CombinedDirection.Y = OpposingDirection.Y == 0 ? SelectedDirection.Y : OpposingDirection.Y;

    // Sides refers to the num of nodes on each row or column, cycles have 4 or 8 nodes so sides are either 2 or 3
    int Side = 2;
    if (Cycle->Nodes.Num() > 4)
    {
        Side = 3;
    }

    // Loop through the array in the given directions if any element is found to contain a node set bIsClear to False
    bool bIsClear = true;
    for (int Xindex = CycleX; Xindex != CycleX + (CombinedDirection.X * 2); Xindex += CombinedDirection.X)
        for (int Yindex = CycleY; Yindex != CycleY + (CombinedDirection.Y * 2); Yindex += CombinedDirection.Y)
        {
            if (Grid[Xindex][Yindex] != nullptr && Grid[Xindex][Yindex] != Cycle->Nodes[0])
            {
                bIsClear = false;
                break;
            }
        }

    // If it isn't clear check the other direction for SelectedDirection
    if (!bIsClear)
    {
        // Select the other direction
        SelectedDirection = FIntPoint(XDirections[Directions[0]], YDirections[Directions[0]]);
        Directions.RemoveAt(RandomIndex);

        // Combine the Opposing and new selected direction
        CombinedDirection = FIntPoint::ZeroValue;
        CombinedDirection.X = OpposingDirection.X == 0 ? SelectedDirection.X : OpposingDirection.X;
        CombinedDirection.Y = OpposingDirection.Y == 0 ? SelectedDirection.Y : OpposingDirection.Y;

        // Loop through the array in the given directions if any element is found to contain a node set bIsClear to False
        bIsClear = true;
        for (int Xindex = CycleX; Xindex != CycleX + (CombinedDirection.X * 2); Xindex += CombinedDirection.X)
            for (int Yindex = CycleY; Yindex != CycleY + (CombinedDirection.Y * 2); Yindex += CombinedDirection.Y) 
            {
                if (Grid[Xindex][Yindex] != nullptr && Grid[Xindex][Yindex] != Cycle->Nodes[0])
                {
                    // If both Selected Directions returned false then return false so that the layout composite may try again
                    return false;
                }
            }
    }

    if (bIsClear)
    {
        // To lay the nodes out in the cyclic layout the nodes needs to be placed along the edges of the clear square
        // The DirectionOrder array holds the order of directions to move to place the nodes - the first direction has to be one of the combined directions
        TArray<FIntPoint> DirectionOrder;

        // Select one of the combined directions add it as the first direction, then add the Selected direction
        int Chosen = FMath::RandRange(0, 1);
        DirectionOrder.Add(Chosen == 0 ? OpposingDirection : SelectedDirection);
        DirectionOrder.Add(Chosen == 0 ? SelectedDirection : OpposingDirection);

        // Then choose the direction that opposes the First Direction
        DirectionOrder.Add(GetOpposingDirection(DirectionOrder[0]));

        // Then choose the direction that opposes the Second Direction
        DirectionOrder.Add(GetOpposingDirection(DirectionOrder[1]));

        int CycleIndex = 1;
        for (FIntPoint Direction : DirectionOrder)
            for (int i = 0; i < Side - 1; i++)
            {
                CycleX += Direction.X;
                CycleY += Direction.Y;
                    
                if (Grid[CycleX][CycleY] == nullptr)
                    Grid[CycleX][CycleY] = Cycle->Nodes[CycleIndex];
                 
                if (CycleIndex < 4)
                    CycleIndex++;
            }
        return true;
    }

    return false;
}

FIntPoint UGraphManager::GetOpposingDirection(const FIntPoint& Direction)
{
    TMap<FIntPoint, FIntPoint> OpposingDirections;
    OpposingDirections.Add(FIntPoint(0, 1), FIntPoint(0, -1));
    OpposingDirections.Add(FIntPoint(0, -1), FIntPoint(0, 1));
    OpposingDirections.Add(FIntPoint(1, 0), FIntPoint(-1, 0));
    OpposingDirections.Add(FIntPoint(-1, 0), FIntPoint(1, 0));

    return OpposingDirections[Direction];
}

/* Set the AdjComposite variable of all the composite nodes - Allows for composites to be placed adjacent during layout */
void UGraphManager::SetAdjComposites(ULevelGraph* Level)
{
    /* Aim is to set set the AdjComposite variable of each composite node, so each composite can be placed onto a grid adjacent to the correct composite
    *  Loop through all nodes
    *  if they are a composite node loop through they're edges
    *  if the edge goes to a composite node
    *  Set the AdjComposite of that node to the node with the edge
    */
    UGraph* Graph = Level->OverallLevel;

    for (UGraphNode* Node : Graph->Nodes)
    {
        if (Node->GetType() == ENodeType::start || Node->GetType() == ENodeType::tree || Node->GetType() == ENodeType::cycle || Node->GetType() == ENodeType::hub || Node->GetType() == ENodeType::inject)
        {
            for (UGraphEdge* Edge : Node->GetEdges())
            {
                UGraphNode* To = Edge->GetTo();
                if (To->GetType() == ENodeType::tree || To->GetType() == ENodeType::cycle || To->GetType() == ENodeType::hub)
                {
                    To->AdjComposite = Node;
                }
            }
        }
    }
}

/* Splits the OverallLevel Graph of a given level into its hub, tree and cycle composites - So the graph can be layed out composite by composite */
void UGraphManager::ExtractComposites(ULevelGraph* Level)
{
    /* For each composite node (tree, hub and cycle nodes) perform a DFS that cannot traverse other composite nodes
    *  Add each of these graphs to the Composite TMap using the starting composite node as the key
    */

    UGraph* Graph = Level->OverallLevel;
    TArray<UGraphNode*> Visited;

    for (UGraphNode* Node : Graph->Nodes)
        if (!Visited.Contains(Node) &&  (Node->GetType() == ENodeType::start || Node->GetType() == ENodeType::tree || Node->GetType() == ENodeType::cycle || Node->GetType() == ENodeType::hub))
        {
            UGraph* Composite = NewObject<UGraph>();
            Composite->Nodes.Add(Node);
            for (UGraphEdge* Edge : Node->GetEdges())
                Composite->Edges.Add(Edge);

            for (UGraphEdge* Edge : Node->GetEdges())
            {
                UGraphNode* Next = Edge->GetTo();
                if (!Visited.Contains(Node) && (Next->GetType() != ENodeType::tree || Next->GetType() != ENodeType::cycle || Next->GetType() != ENodeType::hub))
                    DepthFirstCompositeSearch(Next, Visited, Composite);
            }
            Level->Composites.Add(Node, Composite);
        }
}

/* DFS algorithm that collects the nodes of the composite */
void UGraphManager::DepthFirstCompositeSearch(UGraphNode* Node, TArray<UGraphNode*>& Visited, UGraph* Composite)
{
    /* Traverse through the graph recursively adding nodes to the composite 
    *  if the node passed is a composite node returns without adding the node
    *  recursion ends when the current node has no edges to nodes that havent been visited
    */

    // If Node is composite node return
    if (Node->GetType() == ENodeType::tree || Node->GetType() == ENodeType::cycle || Node->GetType() == ENodeType::hub)
        return;

    Visited.Add(Node);
    Composite->Nodes.Add(Node);

    // Adds all edges of the current node to the Composite
    for (UGraphEdge* Edge : Node->GetEdges())
        Composite->Edges.Add(Edge);

    // Loops through the current node edges checking the node they are to if it hasnt been visited call this function passing the node
    for (UGraphEdge* Edge : Node->GetEdges()) {
        UGraphNode* Next = Edge->GetTo();
        if (!Visited.Contains(Next))
            DepthFirstCompositeSearch(Next, Visited, Composite);
    }
}

/* XML Functions */

void UGraphManager::LoadGraphGrammar(const FString& GrammarFile)
{
    FXmlFile* XmlFile = new FXmlFile(FPaths::ProjectConfigDir() + GrammarFile);
    if (!XmlFile->IsValid()) return;

    const FXmlNode* Root = XmlFile->GetRootNode();
    const FXmlNode* Element = Root->GetFirstChildNode();

    while (Element != nullptr)
    {
        auto Type = Element->GetAttribute("Type");
        if      (Type == "Graph")   ParseGraphs(Element);
        else if (Type == "Rule")    ParseRules(Element);
        else if (Type == "Recipe")  ParseRecipes(Element);

        Element = Element->GetNextNode();
    }
}

void UGraphManager::ParseGraphs(const FXmlNode* Root)
{
    auto Element = Root->GetFirstChildNode();
    while (Element != nullptr)
    {
        FString Name = Element->GetAttribute("Name");
        FString Node = Element->GetAttribute("Node");
        FString Edge = Element->GetAttribute("Edge");

        UGraph* Graph = NewObject<UGraph>();
        Graph->SetNodesString(Node);
        Graph->SetEdgesString(Edge);

        Subgraphs.Add(Name, Graph);

        Element = Element->GetNextNode();
    }
}

void UGraphManager::ParseRules(const FXmlNode* Root)
{
    auto Elements = Root->GetChildrenNodes();
    for (auto CurrentRule : Elements)
    {
        FString Name = CurrentRule->GetAttribute("Name");
        FString Left = CurrentRule->GetAttribute("Left");
        FString Mode = CurrentRule->GetAttribute("Mode");

        TArray<FString> RightNames;
        auto GraphNames = CurrentRule->GetChildrenNodes();
        for (auto Right : GraphNames)
        {
           RightNames.Add(Right->GetAttribute("Name"));
        }

       URule* Rule = NewObject<URule>();
       Rule->Initialise(Left, RightNames, Mode);

       Rules.Add(Name, Rule);
    }
}

void UGraphManager::ParseRecipes(const FXmlNode* Root)
{
    auto Element = Root->GetChildrenNodes();
    for (auto CurrentRecipe : Element)
    {
        FString Name = CurrentRecipe->GetAttribute("name");

        TArray<FString> RuleNames;
        auto RecipeRules = CurrentRecipe->GetChildrenNodes();
        for (auto Rule : RecipeRules)
        {
            RuleNames.Add(Rule->GetAttribute("name"));
        }

       URecipe* Recipe = NewObject<URecipe>();
       Recipe->Initialise(Name, RuleNames);

       Recipes.Add(Name, Recipe);
    }
}
