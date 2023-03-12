// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnTileOverlap.h"
#include "GoalOverlap.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_ABATTY1_API AGoalOverlap : public AOnTileOverlap
{
	GENERATED_BODY()
	
protected:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};
