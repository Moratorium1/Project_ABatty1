// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Rule.generated.h"

UCLASS()
class PROJECT_ABATTY1_API URule : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void Initialise(FString LeftName, TArray<FString> RightNames, FString ModeName);

	UFUNCTION()
	FString GetLeft();

	UFUNCTION()
	TArray<FString> GetRight();

	UFUNCTION()
	FString GetMode();

private:

	UPROPERTY()
	FString Left = "";

	/* Holds the names of graphs that can be selected - */
	UPROPERTY()
	TArray<FString> Right;

	UPROPERTY()
	FString Mode = "";
};
