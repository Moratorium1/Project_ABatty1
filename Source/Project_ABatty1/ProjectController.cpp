// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectController.h"
#include "ProjectCharacter.h"
#include "ProjectGameInstance.h"

void AProjectController::BeginPlay()
{
	Super::BeginPlay();

}

void AProjectController::SetupInputComponent()
{
	Super::SetupInputComponent();

	/* Movement binds */
	InputComponent->BindAxis("MoveForwards", this, &AProjectController::CallMoveForwards);
	InputComponent->BindAxis("MoveStrafe", this, &AProjectController::CallMoveStrafe);
	InputComponent->BindAxis("MoveStrafe", this, &AProjectController::CallMoveStrafe);
	InputComponent->BindAction("Dash", IE_Pressed, this, &AProjectController::CallDash);

	/* Action binds */
	InputComponent->BindAction("Interact", IE_Pressed, this, &AProjectController::CallInteract);

	/* Level generation binds  */
	InputComponent->BindAction("NextLevel", IE_Pressed, this, &AProjectController::CallNextLevel);
	InputComponent->BindAction("AddQuest", IE_Pressed, this, &AProjectController::AddQuestToInjectionQueue);

	/* Player Type binds */
	InputComponent->BindAction("Killer", IE_Pressed, this, &AProjectController::CallIncreaseKillerType);
	InputComponent->BindAction("Achiever", IE_Pressed, this, &AProjectController::CallIncreaseAchieverType);
	InputComponent->BindAction("Socialiser", IE_Pressed, this, &AProjectController::CallIncreaseSocialiserType);
	InputComponent->BindAction("Explorer", IE_Pressed, this, &AProjectController::CallIncreaseExplorerType);

}

void AProjectController::CallMoveForwards(float AxisAmount)
{
	if (GetPawn() == nullptr) return;

	if (GetPawn()->IsA(AProjectCharacter::StaticClass()))
		Cast<AProjectCharacter>(GetPawn())->MoveForwards(AxisAmount);

}

void AProjectController::CallMoveStrafe(float AxisAmount)
{
	if (GetPawn() == nullptr) return;

	if (GetPawn()->IsA(AProjectCharacter::StaticClass()))
		Cast<AProjectCharacter>(GetPawn())->MoveStrafe(AxisAmount);
}

void AProjectController::CallDash()
{
	if (GetPawn() == nullptr) return;

	if (GetPawn()->IsA(AProjectCharacter::StaticClass()))
		Cast<AProjectCharacter>(GetPawn())->Dash();
}

void AProjectController::CallInteract()
{
	if (GetPawn() == nullptr) return;

	if (GetPawn()->IsA(AProjectCharacter::StaticClass()))
		Cast<AProjectCharacter>(GetPawn())->Interact();
}

void AProjectController::CallIncreaseKillerType()
{
	if (GetGameInstance() == nullptr) return;

	if (GetGameInstance()->IsA(UProjectGameInstance::StaticClass()))
		Cast<UProjectGameInstance>(GetGameInstance())->IncreaseKillerType();
}

void AProjectController::CallIncreaseAchieverType()
{
	if (GetGameInstance() == nullptr) return;

	if (GetGameInstance()->IsA(UProjectGameInstance::StaticClass()))
		Cast<UProjectGameInstance>(GetGameInstance())->IncreaseAchieverType();
}

void AProjectController::CallIncreaseSocialiserType()
{
	if (GetGameInstance() == nullptr) return;

	if (GetGameInstance()->IsA(UProjectGameInstance::StaticClass()))
		Cast<UProjectGameInstance>(GetGameInstance())->IncreaseSocialiserType();
}

void AProjectController::CallIncreaseExplorerType()
{
	if (GetGameInstance() == nullptr) return;

	if (GetGameInstance()->IsA(UProjectGameInstance::StaticClass()))
		Cast<UProjectGameInstance>(GetGameInstance())->IncreaseExplorerType();
}

void AProjectController::CallNextLevel()
{
	if (GetGameInstance() == nullptr) return;

	if (GetGameInstance()->IsA(UProjectGameInstance::StaticClass()))
		Cast<UProjectGameInstance>(GetGameInstance())->NextLevel();
}

void AProjectController::AddQuestToInjectionQueue()
{
	if (GetGameInstance() == nullptr) return;

	if (GetGameInstance()->IsA(UProjectGameInstance::StaticClass()))
		Cast<UProjectGameInstance>(GetGameInstance())->InjectQuest();
}
