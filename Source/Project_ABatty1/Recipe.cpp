// Fill out your copyright notice in the Description page of Project Settings.


#include "Recipe.h"

void URecipe::Initialise(const FString& RecipeName, TArray<FString> RuleNames)
{
	Name = RecipeName;

	SetRules(RuleNames);
}

void URecipe::SetRules(const TArray<FString> RuleNames)
{
	Rules = RuleNames;
}

TArray<FString> URecipe::GetRules()
{
	return Rules;
}

