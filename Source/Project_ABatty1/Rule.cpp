// Fill out your copyright notice in the Description page of Project Settings.


#include "Rule.h"
#include "ProjectGameInstance.h"

void  URule::Initialise(FString LeftName, TArray<FString> RightNames, FString ModeName)
{
	Left = LeftName;

	Right = RightNames;

	Mode = ModeName;
}

FString URule::GetLeft()
{
	return Left;
}

TArray<FString> URule::GetRight()
{
	return Right;
}

FString URule::GetMode()
{
	return Mode;
}