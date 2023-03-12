// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectCharacter.generated.h"

class AInteractive;
class APickUp;

UCLASS()
class PROJECT_ABATTY1_API AProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AProjectCharacter();

	friend class AProjectController;

	USceneComponent* GetPickupComponent();

	UFUNCTION()
	void SetPickUp(APickUp* Pickup);

	UFUNCTION()
	APickUp* GetPickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(EditAnywhere, Category = "Model")
	UStaticMeshComponent* Model;

	UPROPERTY(EditAnywhere, Category = "Tile")
	TSubclassOf<AActor> StartClass;

	/* Camera */
	UPROPERTY(VisibleAnywhere, Category = "SpringArm")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category = "SpringArm")
	float KArmLength = 500.0f;

	UPROPERTY(EditAnywhere, Category = "SpringArm")
	float KArmPitch = -50.0f;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* Camera;

	/* Movement Variables */
	UPROPERTY(EditAnywhere, Category = "Movement")
	float KRotationRate = 540.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float KDashPower = 100.0f;

	/* Movement Functions */ 

	UFUNCTION()
	void MoveForwards(float AxisAmount);

	UFUNCTION()
	void MoveStrafe(float AxisAmount);

	UFUNCTION()
	void Dash();

	/* Interaction Functions & Variables */

	/* Item Variables */
	UPROPERTY(EditAnywhere, Category = "Item")
	APickUp* PickUp = nullptr;

	/* Position a held pickup should attach */
	UPROPERTY(EditAnywhere, Category = "Item")
	USceneComponent* PickupPosition;

	UPROPERTY(EditAnywhere, Category = "Item")
	float PickupXPosition = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Item")
	float PickupZPosition = 40.0f;

	/* Interact LineTrace Variables */
	/* The range of the interact line trace */
	UPROPERTY(EditAnywhere, Category = "Interact")
	float KInteractRange = 150.0f;

	/* The radius of the interact line trace */
	UPROPERTY(EditAnywhere, Category = "Interact")
	float KInteractRadius = 15.0f;

	UFUNCTION()
	void Interact();

	UFUNCTION()
	AInteractive* InteractRangeCheck();

	void PickUpInteraction(APickUp* Pickup);

};
