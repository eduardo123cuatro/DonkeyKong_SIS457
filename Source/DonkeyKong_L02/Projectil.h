// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectil.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS(config = Game)
class AProjectil : public AActor
{
	GENERATED_BODY()

	/** Static mesh component for the projectile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectil, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectilMesh;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectilMovement;

	UPROPERTY()
	UMaterialInstanceDynamic* ProjectilMaterial;

public:
	AProjectil();

	/** Function to handle the projectile hitting something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns ProjectilMesh subobject **/
	FORCEINLINE UStaticMeshComponent* GetProjectilMesh() const { return ProjectilMesh; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE UProjectileMovementComponent* GetProjectilMovement() const { return ProjectilMovement; }
};

