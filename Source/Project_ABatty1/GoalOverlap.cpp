// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalOverlap.h"
#include "ProjectGameInstance.h"

void AGoalOverlap::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Cast<UProjectGameInstance>(GetGameInstance())->NextLevel();
}