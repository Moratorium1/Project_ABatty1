// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnTile.h"
#include "PlayerSpawnTile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_ABATTY1_API APlayerSpawnTile : public ASpawnTile
{
	GENERATED_BODY()
	
protected:

	virtual void SpawnOnTile() override; 
};
