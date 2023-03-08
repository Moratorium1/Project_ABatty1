// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Recipe.generated.h"

UCLASS()
class PROJECT_ABATTY1_API URecipe : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
		void Initialise(const FString& RecipeName, TArray<FString> RuleNames);

	UFUNCTION()
		void SetRules(const TArray<FString> RuleNames);

	UFUNCTION()
		TArray<FString> GetRules();

private:

	UPROPERTY()
		FString Name = "";

	UPROPERTY()
		TArray<FString> Rules;
};
