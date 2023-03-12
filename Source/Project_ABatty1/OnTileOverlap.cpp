// Fill out your copyright notice in the Description page of Project Settings.


#include "OnTileOverlap.h"
#include "Components/BoxComponent.h"

AOnTileOverlap::AOnTileOverlap()
{
    OverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MyBoxComponent"));
    RootComponent = OverlapComponent;
}

void AOnTileOverlap::BeginPlay()
{
    Super::BeginPlay();

    OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &AOnTileOverlap::OnOverlapBegin);
}

void AOnTileOverlap::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
