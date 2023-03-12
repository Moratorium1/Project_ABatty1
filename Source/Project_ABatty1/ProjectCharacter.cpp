// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Interactive.h"
#include "PickUp.h"



// Sets default values
AProjectCharacter::AProjectCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* Model Setup */
	Model = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model"));
	Model->SetupAttachment(RootComponent);

	/* Camera Setup */
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	/* SpringArm Variables */
	SpringArm->TargetArmLength = KArmLength;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bInheritPitch = false;

	SpringArm->SetRelativeRotation(FRotator(KArmPitch, 0, 0));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	/* Movement Setup */
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, KRotationRate, 0.0f);

	/* Held Item Setup */
	PickupPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Pickup Position"));
	PickupPosition->SetupAttachment(RootComponent);
	PickupPosition->SetRelativeLocation(FVector(PickupXPosition, 0, PickupZPosition));

}

USceneComponent* AProjectCharacter::GetPickupComponent()
{
	return PickupPosition;
}

// Called when the game starts or when spawned
void AProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* Start = UGameplayStatics::GetActorOfClass(GetWorld(), StartClass);

	if (Start != nullptr)
	{
		FVector StartLocation = Start->GetActorLocation();
		FVector PlayerStartLocation = FVector(StartLocation.X, StartLocation.Y, StartLocation.Z + 100);

		SetActorLocation(PlayerStartLocation);
	}
}

// Called every frame
void AProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AProjectCharacter::MoveForwards(float AxisAmount)
{
	if ((Controller != nullptr) && (AxisAmount != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisAmount);
	}
}

void AProjectCharacter::MoveStrafe(float AxisAmount)
{
	if ((Controller != nullptr) && (AxisAmount != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, AxisAmount);
	}
}


void AProjectCharacter::Dash()
{
	LaunchCharacter(GetActorForwardVector() * KDashPower, false, false);
}

void AProjectCharacter::Interact()
{
	AInteractive* Interactive = InteractRangeCheck();

	if (Interactive == nullptr) return;

	Interactive->Interact(this);
}

AInteractive* AProjectCharacter::InteractRangeCheck()
{
	AInteractive* Interactive = nullptr;

	FVector Start = GetActorLocation();
	FRotator Direction = GetActorRotation();
	FVector End = Start + Direction.Vector() * KInteractRange;

	FHitResult Result;
	TArray<AActor*> Ignore;
	Ignore.Add(this);
	Ignore.Add(PickUp);
	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, KInteractRadius, ETraceTypeQuery::TraceTypeQuery1, false, Ignore, EDrawDebugTrace::ForOneFrame, Result, true, FColor::Red, FColor::Green))
		Interactive = Cast<AInteractive>(Result.GetActor());

	return Interactive;
}

void AProjectCharacter::SetPickUp(APickUp* Pickup)
{
	PickUp = Pickup;
}

APickUp* AProjectCharacter::GetPickUp()
{
	return PickUp;
}

void AProjectCharacter::PickUpInteraction(APickUp* Pickup)
{

}
