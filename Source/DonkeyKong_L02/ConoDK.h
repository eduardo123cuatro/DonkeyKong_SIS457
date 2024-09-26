#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConoDK.generated.h"

class UStaticMeshComponent;
class AProjectil;

UCLASS()
class DONKEYKONG_L02_API AConoDK : public AActor
{
	GENERATED_BODY()

public:
	AConoDK();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	//UPROPERTY(EditDefaultsOnly, Category = "Config")
	//TSubclassOf<AProjectil> ProjectilClass; // Clase de proyectil a disparar

	//UPROPERTY(EditDefaultsOnly, Category = "Config")
	//float DisparoIntervalo;

	//FTimerHandle TimerHandle_Disparo;

	/*void DispararProjectil();*/
};
