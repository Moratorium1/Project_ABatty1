// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactive.h"
#include "ProjectCharacter.h"
#include "Components/SphereComponent.h"


AInteractive::AInteractive()
{
	/* Model Setup */
	Model = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model"));
	Model->SetupAttachment(RootComponent);

	/* Collision Setup */
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(Model);
	Collision->SetSphereRadius(16, false);
	Collision->SetRelativeLocation(FVector(0, 0, 35.0f));
}

void AInteractive::Interact(AProjectCharacter* Player)
{

}