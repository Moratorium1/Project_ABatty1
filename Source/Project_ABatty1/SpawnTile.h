// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "SpawnTile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_ABATTY1_API ASpawnTile : public ATile
{
	GENERATED_BODY()
	
public:

	ASpawnTile();

	/* The Actor to spawn on top of the tile */
	UPROPERTY(EditAnywhere, Category = "OnTile")
	TSubclassOf<AActor> OnTileClass;

protected:

	virtual void BeginPlay() override;

	/* Spawns the OnTile Actor at the OnTilePosition */
	UFUNCTION()
	virtual void SpawnOnTile();

	/* The location to spawn the on tile actor  */
	UPROPERTY(EditAnywhere, Category = "Item", Meta = (MakeEditWidget = true))
	USceneComponent* OnTilePosition;

private:

	/* The Actor to spawn on top of the actor */
	UPROPERTY()
	AActor* OnTile = nullptr;

	UPROPERTY(EditAnywhere, Category = "Item")
	float OnTileZPosition = 300.0f;

};
