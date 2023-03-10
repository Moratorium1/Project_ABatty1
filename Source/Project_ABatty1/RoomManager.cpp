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

        default:
            Room = GenerateRoom(Level, RoomKey);
        }

        Level->Rooms[RoomKey] = Room;
    }
}

TArray<TArray<ETileType>> URoomManager::GenerateStartRoom(ULevelGraph* Level, UGraphNode* RoomKey)
{
    TArray<TArray<ETileType>> Room = Level->Rooms[RoomKey];
    Room = GenerateRoom(Level, RoomKey);

    Room[RoomSize / 2][RoomSize / 2] = ETileType::START;
    
    return Room;
}

TArray<TArray<ETileType>> URoomManager::GenerateRoom(ULevelGraph* Level, UGraphNode* RoomKey)
{
    TArray<TArray<ETileType>> Room = Level->Rooms[RoomKey];

    for (int x = 0; x < RoomSize; x++)
        for (int y = 0; y < RoomSize; y++)
            Room[x][y] = ETileType::FLOOR;

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
    */

    TArray<TArray<ETileType>> Room = Level->Rooms[RoomKey];
    FIntPoint EdgeDirection = RoomKey->EdgeDirection;

    int Centre = 4;
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