// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactive.h"
#include "PickUp.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_ABATTY1_API APickUp : public AInteractive
{
	GENERATED_BODY()

public:

	virtual void Interact(AProjectCharacter* Player) override;

};
