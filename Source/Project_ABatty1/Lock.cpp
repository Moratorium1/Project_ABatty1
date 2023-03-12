// Fill out your copyright notice in the Description page of Project Settings.


#include "Lock.h"
#include "PickUpKey.h"
#include "ProjectCharacter.h"

void ALock::Interact(AProjectCharacter* Player)
{
	if (Cast<APickUp>(Player->GetPickUp()))
	{
		Player->GetPickUp()->Destroy();
		Destroy();
		Player->SetPickUp(nullptr);
	}
}