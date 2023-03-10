// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectGameInstance.h"
#include "ProjectController.generated.h"

UCLASS()
class PROJECT_ABATTY1_API AProjectController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void SetupInputComponent() override;

private:

	UFUNCTION()
	void CallMoveForwards(float AxisAmount);

	UFUNCTION()
	void CallMoveStrafe(float AxisAmount);

	UFUNCTION()
	void CallDash();

	UFUNCTION()
	void CallIncreaseKillerType();

	UFUNCTION()
	void CallIncreaseAchieverType();

	UFUNCTION()
	void CallIncreaseSocialiserType();

	UFUNCTION()
	void CallIncreaseExplorerType();

	UFUNCTION()
	void CallNextLevel();

	UFUNCTION()
	void AddQuestToInjectionQueue();

};
