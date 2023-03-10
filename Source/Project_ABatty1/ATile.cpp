// Fill out your copyright notice in the Description page of Project Settings.


#include "ATile.h"

// Sets default values
AATile::AATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(TileMesh);
}

// Called when the game starts or when spawned
void AATile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

