// Copyright Epic Games, Inc. All Rights Reserved.

#include "Projectil.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

AProjectil::AProjectil()
{
	// Static reference to the mesh to use for the projectile
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectilMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_WideCapsule.Shape_WideCapsule'"));

	// Create mesh component for the projectile sphere
	ProjectilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectilMesh"));
	ProjectilMesh->SetStaticMesh(ProjectilMeshAsset.Object);
	ProjectilMesh->SetupAttachment(RootComponent);
	ProjectilMesh->BodyInstance.SetCollisionProfileName("Projectile");
	ProjectilMesh->OnComponentHit.AddDynamic(this, &AProjectil::OnHit); // Set up a notification for when this component hits something
	SetRootComponent(ProjectilMesh);
	ProjectilMesh->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f)); // Adjust the size of the projectile here

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectilMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectilMovement->UpdatedComponent = ProjectilMesh;
	ProjectilMovement->InitialSpeed = 2000.f; // Initial speed of the projectile
	ProjectilMovement->MaxSpeed = 2000.f; // Maximum speed
	ProjectilMovement->bRotationFollowsVelocity = true;
	ProjectilMovement->bShouldBounce = false;
	ProjectilMovement->ProjectileGravityScale = 0.f; // No gravity

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AProjectil::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics object
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 20.0f, GetActorLocation());
	}

	Destroy();
}
