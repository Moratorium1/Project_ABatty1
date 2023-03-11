// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSpawnTile.h"
#include "ProjectCharacter.h"
#include "Kismet/GameplayStatics.h"

void APlayerSpawnTile::SpawnOnTile()
{
	/* Check if the player character already exists if no spawn them as usual
	*  Else Move the player ot the spawn tile location	
	*/

	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), OnTileClass, FoundCharacters);

	if (FoundCharacters.IsEmpty())
		Super::SpawnOnTile();
	else
		FoundCharacters[0]->SetActorLocation(OnTilePosition->GetComponentLocation());
}
