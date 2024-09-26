#include "ConoDK.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "TimerManager.h"

AConoDK::AConoDK()
{
	PrimaryActorTick.bCanEverTick = true;

	// Crear y configurar el componente de malla est�tica
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	// Asignar una malla al componente de malla est�tica
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone'")); // Cambia esta ruta a la ubicaci�n de tu malla
	if (MeshAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshAsset.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No se encontr� la malla para el ConoDK."));
	}

	//DisparoIntervalo = 2.0f; // Intervalo de disparo en segundos
}

void AConoDK::BeginPlay()
{
	Super::BeginPlay();

	//if (ProjectilClass)
	//{
	//	// Iniciar el temporizador para disparar proyectiles a intervalos regulares
	//	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Disparo, this, &AConoDK::DispararProjectil, DisparoIntervalo, true);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("No se ha asignado ninguna clase de proyectil para el ConoDK."));
	//}
}

void AConoDK::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//void AConoDK::DispararProjectil()
//{
//	if (ProjectilClass)
//	{
//		// Obtiene la ubicaci�n y la rotaci�n del cono
//		FVector SpawnLocation = GetActorLocation();
//		FRotator SpawnRotation = FRotator(-90.0f, 0.0f, 0.0f); // Orientaci�n hacia abajo
//
//		// Genera el proyectil
//		AProjectil* Proyectil = GetWorld()->SpawnActor<AProjectil>(ProjectilClass, SpawnLocation, SpawnRotation);
//		if (Proyectil)
//		{
//			// Opcional: Configura el proyectil si es necesario
//		}
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("No se ha asignado ninguna clase de proyectil al disparar desde ConoDK."));
//	}
//}
