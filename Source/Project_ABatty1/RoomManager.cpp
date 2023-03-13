// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomManager.h"
#include "ProjectGameInstance.h"
#include "LevelGraph.h"
#include "Graph.h"
#include "GraphNode.h"


void URoomManager::Initialise()
{
    GameInstance = Cast<UProjectGameInstance>(GetGameInstance());

    RoomSize = GameInstance->RoomSize;

}

void URoomManager::ExtractRooms(ULevelGraph* Level)
{
    /* Loop thorugh the CoarseGrid - Has to be CoarseGrid and not just the Nodes of the Level as only the CoarseGrid contains the Edges
    *  For each node initialise a 2D array of tile types
    *  Add the 2D array to the Rooms TMap with the Node as the Key
    */

    TArray<TArray<UGraphNode*>> CoarseGrid = Level->CoarseGrid;

    for (int x = 0; x < CoarseGrid.Num(); x++)
        for (int y = 0; y < CoarseGrid.Num(); y++)
        {
            UGraphNode* CurrentNode = CoarseGrid[x][y];

            if (CurrentNode != nullptr)
                if (!Level->Rooms.Contains(CurrentNode))
                {
                    TArray<TArray<ETileType>> Room;

                    /* Set up the room to the correct size */
                    Room.SetNumZeroed(RoomSize);
                    for (int i = 0; i < RoomSize; i++)
                        Room[i].Init(ETileType::EMPTY, RoomSize);

                    Level->Rooms.Add(CurrentNode, Room);
                }
        }
}

void URoomManager::GenerateRooms(ULevelGraph* Level)
{
    TArray<UGraphNode*> RoomKeys;
    Level->Rooms.GetKeys(RoomKeys);

    for (UGraphNode* RoomKey : RoomKeys)
    {
        TArray<TArray<ETileType>> Room;

        switch (RoomKey->GetType())
        {
        case ENodeType::start:
            Room = GenerateStartRoom(Level, RoomKey);
            break;

        case ENodeType::ROOM:
            Room = GenerateRoom(Level, RoomKey);
                break;

        case ENodeType::EDGE:
            Room = GenerateEdge(Level, RoomKey);
            break;

        case ENodeType::BOSS:
            Room = GenerateBossRoom(Level, RoomKey);
            break;

        case ENodeType::GOAL:
            Room = GenerateGoalRoom(Level, RoomKey);
            break;

        case ENodeType::LOCKED_EDGE:
            Room = GenerateLockedEdge(Level, RoomKey);
            break;

        case ENodeType::KEY:
            Room = GenerateKeyRoom(Level, RoomKey);
            break;

        case ENodeType::KILLER:
            Room = GeneratePlayerTypeRoom(Level, RoomKey);
            break;

        case ENodeType::ACHIEVER:
            Room = GeneratePlayerTypeRoom(Level, RoomKey);
            break;

        case ENodeType::SOCIALISER:
            Room = GeneratePlayerTypeRoom(Level, RoomKey);
            break;

        case ENodeType::EXPLORER:
            Room = GeneratePlayerTypeRoom(Level, RoomKey);
            break;


        default:
            Room = GenerateRoom(Level, RoomKey);
        }

        Level->Rooms[RoomKey] = Room;
    }
}

/* Create a basic room with all tiles of the room set as floor tiles */
TArray<TArray<ETileType>> URoomManager::GenerateBaseRoom(ULevelGraph* Level, UGraphNode* RoomKey)
{
    TArray<TArray<ETileType>> Room = Level->Rooms[RoomKey];

    for (int x = 0; x < RoomSize; x++)
        for (int y = 0; y < RoomSize; y++)
            Room[x][y] = ETileType::FLOOR;

    return Room;
}

/* Create the basic room then add a start tile */
TArray<TArray<ETileType>> URoomManager::GenerateStartRoom(ULevelGraph* Level, UGraphNode* RoomKey)
{
    TArray<TArray<ETileType>> Room = Level->Rooms[RoomKey];
    Room = GenerateBaseRoom(Level, RoomKey);

    Room[RoomSize / 2][RoomSize / 2] = ETileType::START;
    
    return Room;
}

/* Create the basic room then add a key tile */
TArray<TArray<ETileType>> URoomManager::GenerateKeyRoom(ULevelGraph* Level, UGraphNode* RoomKey)
{
    TArray<TArray<ETileType>> Room = Level->Rooms[RoomKey];
    Room = GenerateBaseRoom(Level, RoomKey);

    Room[RoomSize / 2][RoomSize / 2] = ETileType::KEY;

    return Room;
}

/* Create the basic room then add a player type specific tile in the centre */
TArray<TArray<ETileType>> URoomManager::GeneratePlayerTypeRoom(ULevelGraph* Level, UGraphNode* RoomKey)
{
    TArray<TArray<ETileType>> Room = Level->Rooms[RoomKey];
    Room = GenerateBaseRoom(Level, RoomKey);

    ETileType PlayerType = ETileType::INVALID;

    switch (RoomKey->GetType())
    {
    case ENodeType::KILLER:
        PlayerType = ETileType::KILLER;
        break;

    case ENodeType::ACHIEVER:
        PlayerType = ETileType::ACHIEVER;
        break;

    case ENodeType::SOCIALISER:
        PlayerType = ETileType::SOCIALISER;
        break;

    case ENodeType::EXPLORER:
        PlayerType = ETileType::EXPLORER;
        break;
    }

    Room[RoomSize / 2][RoomSize / 2] = PlayerType;

    return Room;
}

/* Generate the basic room then add 1 - 3 enemy spawn tiles */
TArray<TArray<ETileType>> URoomManager::GenerateRoom(ULevelGraph* Level, UGraphNode* RoomKey)
{
    TArray<TArray<ETileType>> Room = Level->Rooms[RoomKey];
    Room = GenerateBaseRoom(Level, RoomKey);

    int NumEnemies = FMath::RandRange(1, 3);

    for (int i = 0; i < NumEnemies; i++)
    {
        int X = FMath::RandRange(0, Room.Num() - 1);
        int Y = FMath::RandRange(0, Room.Num() - 1);

        Room[X][Y] = ETileType::ENEMY;
    }

    return Room;
}

/* Generate basic room then add a boss spawning tile in the centre */
TArray<TArray<ETileType>> URoomManager::GenerateBossRoom(ULevelGraph* Level, UGraphNode* RoomKey)
{
    TArray<TArray<ETileType>> Room = Level->Rooms[RoomKey];
    Room = GenerateBaseRoom(Level, RoomKey);

    Room[RoomSize / 2][RoomSize / 2] = ETileType::BOSS;

    return Room;
}

TArray<TArray<ETileType>> URoomManager::GenerateEdge(ULevelGraph* Level, UGraphNode* RoomKey)
{
    /*
    *  Edge are thin rooms that connect Regular rooms
    *  The EdgeDirection gives the direction from the room to the next room
    *  There are four possible directions but only 2 different outcomes for the room layout
    *       - A Corridor running from North to South
    *       - A Corridor running from East to West
    *  These corridors will be placed centrally in the room
    * 
    * 
    */

    TArray<TArray<ETileType>> Room = Level->Rooms[RoomKey];
    FIntPoint EdgeDirection = RoomKey->EdgeDirection;

    int Centre = RoomSize / 2;
    if (EdgeDirection.X == 0)
    {
        /* The corridor needs to run east to west */
        for (int y = 0; y < RoomSize; y++)
            Room[Centre][y] = ETileType::FLOOR;
    }
    else if (EdgeDirection.Y == 0)
    {
        /* The corridor need to run north to south */
        for (int x = 0; x < RoomSize; x++)
            Room[x][Centre] = ETileType::FLOOR;
    }

    return Room;
}

/* Create an edge room then add the lock */
TArray<TArray<ETileType>> URoomManager::GenerateLockedEdge(ULevelGraph* Level, UGraphNode* RoomKey)
{
    /* Locked Edges are edges with the first tile from the FromNode room being a LOCK tile
    *  Generate an edge as usual then set the first tile as the lock tile
    */

    TArray<TArray<ETileType>> Room = Level->Rooms[RoomKey];
    FIntPoint EdgeDirection = RoomKey->EdgeDirection;
    Room = GenerateEdge(Level, RoomKey);

    int Centre = RoomSize / 2;
    if (EdgeDirection.X == 0)
        Room[Centre][0] = ETileType::LOCK;
    else if (EdgeDirection.Y == 0)
        Room[0][Centre] = ETileType::LOCK;

    return Room;
}

/* Create a basic room then add a Goal tile at the centre */
TArray<TArray<ETileType>> URoomManager::GenerateGoalRoom(ULevelGraph* Level, UGraphNode* RoomKey)
{
    TArray<TArray<ETileType>> Room = Level->Rooms[RoomKey];
    Room = GenerateBaseRoom(Level, RoomKey);

    Room[RoomSize / 2][RoomSize / 2] = ETileType::GOAL;

    return Room;
}
