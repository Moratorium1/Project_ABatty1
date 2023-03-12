// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnTile.h"
#include "OnTileOverlap.generated.h"

class UBoxComponent;

UCLASS()
class PROJECT_ABATTY1_API AOnTileOverlap : public AOnTile
{
	GENERATED_BODY()
	
public:

	AOnTileOverlap();

protected:

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* OverlapComponent;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

};
