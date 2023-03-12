// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"
#include "ProjectCharacter.h"

void APickUp::Interact(AProjectCharacter* Player)
{
	AttachToComponent(Player->GetPickupComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Player->SetPickUp(this);
}