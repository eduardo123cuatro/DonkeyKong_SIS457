// Copyright Epic Games, Inc. All Rights Reserved.

#include "DonkeyKong_L02Character.h"
#include "Projectil.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

const FName ADonkeyKong_L02Character::FireRightBinding("FireRight");

ADonkeyKong_L02Character::ADonkeyKong_L02Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1500.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	bCanFire = true;
	FireRate = 0.3f;
	GunOffset = FVector(0.f, 90.f, 0.f);

}

//////////////////////////////////////////////////////////////////////////
// Input

void ADonkeyKong_L02Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADonkeyKong_L02Character::MoveRight);
	PlayerInputComponent->BindAxis("FireRight", this, &ADonkeyKong_L02Character::FireRight);
	PlayerInputComponent->BindAxis("FireLeft", this, &ADonkeyKong_L02Character::FireLeft);
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ADonkeyKong_L02Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ADonkeyKong_L02Character::TouchStopped);
}

void ADonkeyKong_L02Character::BeginPlay()
{
	Super::BeginPlay();
}

void ADonkeyKong_L02Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	const FVector FireDirection = FVector(0.f, FireRightValue, 0.f);

	// Try and fire a shot
	//FireShot(FireDirection);

}

void ADonkeyKong_L02Character::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
}

void ADonkeyKong_L02Character::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ADonkeyKong_L02Character::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void ADonkeyKong_L02Character::FireShot(FVector _Direction)
{
	// If it's ok to fire again
	if (bCanFire == true)
	{
		// If we are pressing fire stick in a direction
		if (_Direction.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = _Direction.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				// spawn the projectile
				World->SpawnActor<AProjectil>(SpawnLocation + FVector(0.f, 20.f, 0.f), FireRotation);
			}
			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ADonkeyKong_L02Character::ShotTimerExpired, FireRate);
		}
	}
}

void ADonkeyKong_L02Character::ShotTimerExpired()
{
	bCanFire = true;
}

void ADonkeyKong_L02Character::FireRight(float Val)
{
	const FVector FireDirection = FVector(0.f, Val, 0.f);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Derecha: %f"), Val));
	FireShot(FireDirection);
}

void ADonkeyKong_L02Character::FireLeft(float Val)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Izquierda: %f"), Val));
	const FVector FireDirection = FVector(0.f, Val, 0.f);
	FireShot(FireDirection);
}