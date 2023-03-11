// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnTile.h"
#include "ProjectCharacter.h"


ASpawnTile::ASpawnTile() : Super()
{
	/* On Tile Setup */
	OnTilePosition = CreateDefaultSubobject<USceneComponent>(TEXT("Placed Item Position"));
	OnTilePosition->SetupAttachment(TileMesh);
	OnTilePosition->SetRelativeLocation(FVector(0, 0, OnTileZPosition));

}

void ASpawnTile::BeginPlay()
{
	Super::BeginPlay();

	SpawnOnTile();
}

/*  */
void ASpawnTile::SpawnOnTile()
{
	OnTile = GetWorld()->SpawnActor<AActor>(OnTileClass, OnTilePosition->GetComponentLocation(), FRotator::ZeroRotator);
	
	if (OnTile != nullptr)
		OnTile->AttachToComponent(OnTilePosition, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}
