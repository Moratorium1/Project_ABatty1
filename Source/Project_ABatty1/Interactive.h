// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnTile.h"
#include "Interactive.generated.h"

class USphereComponent;
class AProjectCharacter;

UCLASS()
class PROJECT_ABATTY1_API AInteractive : public AOnTile
{
	GENERATED_BODY()
	
public:

	AInteractive();

	virtual void Interact(AProjectCharacter* Player);

private:

	UPROPERTY(EditAnywhere, Category = "Model")
	UStaticMeshComponent* Model;

	UPROPERTY(EditAnywhere, Category = "Collision")
	USphereComponent* Collision;

};
