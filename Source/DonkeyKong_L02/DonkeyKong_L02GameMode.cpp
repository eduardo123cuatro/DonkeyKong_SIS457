#include "DonkeyKong_L02GameMode.h"
#include "DonkeyKong_L02Character.h"
#include "UObject/ConstructorHelpers.h"
#include "componentePlataforma.h"
#include "Barril.h"
#include "Esfera.h"
#include "TimerManager.h"
#include "ConoDK.h"  // Incluir el ConoDK
#include "Kismet/GameplayStatics.h"  // Incluir GameplayStatics

ADonkeyKong_L02GameMode::ADonkeyKong_L02GameMode()
{
	// Establecer la clase del peón predeterminado a nuestro personaje Blueprint
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ADonkeyKong_L02GameMode::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, TEXT("Creando plataforma"));
	
	// Definir una nueva posición de spawn para el personaje
	FVector NuevaPosicionDeSpawn = FVector(1200.0f, 1200.0f, 5000.0f);  // Ajusta la altura (Z) y otras coordenadas
	FRotator RotacionSpawn = FRotator(0.0f, 0.0f, 0.0f);  // Rotación sin cambios

	// Obtener el personaje controlado
	APawn* PlayerCharacter = UGameplayStatics::GetPlayerPawn(this, 0);
	if (PlayerCharacter)
	{
		PlayerCharacter->SetActorLocation(NuevaPosicionDeSpawn); // Mover el personaje a la nueva posición
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("Personaje movido a la nueva posición"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Error: no se encontró el personaje"));
	}


	// Mapa para almacenar las plataformas creadas
	TMap<FString, AcomponentePlataforma*> MapPlataformas;

	// Configuración de las plataformas
	FVector posicionInicial = FVector(1160.0f, -1100.0f, 400.f);
	FRotator rotacionInicial = FRotator(0.0f, 0.0f, 10.0f);
	FTransform SpawnLocationCP;
	float anchoComponentePlataforma = 260.0f;
	float incrementoAltoComponentePlataforma = -55.0f;
	float incrementoAltoEntrePisos = 1000.0f;
	float incrementoInicioPiso = 100.0f;    

	// Variable para la posición final del módulo 5
	FVector posicionUltimoModulo5;

	// Crear plataformas y almacenarlas en el TMap
	for (int npp = 0; npp < 5; npp++) {
		rotacionInicial.Roll = rotacionInicial.Roll * -1;
		incrementoInicioPiso = incrementoInicioPiso * -1;
		incrementoAltoComponentePlataforma = incrementoAltoComponentePlataforma * -1;
		SpawnLocationCP.SetRotation(FQuat(rotacionInicial));

		for (int ncp = 0; ncp < 10; ncp++) {
		/*	bool esPiso2o4 = (npp == 1 || npp == 3);
		
			if (ncp == 0 || ncp == 1 || ncp == 4 || (esPiso2o4 && (ncp == 2 || ncp == 3))) {*/
			if (ncp == 0 || ncp == 1 || ncp == 4 || ncp == 5 || ncp == 6 || ncp == 7 || ncp == 8 || ncp == 9) {
				SpawnLocationCP.SetLocation(FVector(posicionInicial.X, posicionInicial.Y + anchoComponentePlataforma * ncp, posicionInicial.Z));
				AcomponentePlataforma* nuevaPlataforma = GetWorld()->SpawnActor<AcomponentePlataforma>(AcomponentePlataforma::StaticClass(), SpawnLocationCP);
				componentesPlataforma.Add(nuevaPlataforma);

				// Crear una clave única para el TMap utilizando los índices npp (piso) y ncp (módulo)
				FString clave = FString::Printf(TEXT("Piso%d_Modulo%d"), npp + 1, ncp + 1);
				MapPlataformas.Add(clave, nuevaPlataforma);

				// Actualizar la posición del último módulo del piso 5
				if (npp == 10 && ncp == 10) {
					posicionUltimoModulo5 = SpawnLocationCP.GetLocation();
				}

				// Movimiento del módulo 2 en los pisos 1, 3 y 5
				if ((ncp == 1) && (npp == 0 || npp == 2 || npp == 4)) {
					FTimerHandle TimerHandle;
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, [nuevaPlataforma, posicionInicial, anchoComponentePlataforma]() {
						static bool moviendoAdelante = true;
						FVector nuevaPosicion = nuevaPlataforma->GetActorLocation();
						float pasoMovimiento = 5.0f;  // Ajustar la cantidad de movimiento para hacerlo más lento

						if (moviendoAdelante) {
							// Mover hacia el módulo 4
							nuevaPosicion.Y += pasoMovimiento; // Mover un paso pequeño
							if (nuevaPosicion.Y >= posicionInicial.Y + anchoComponentePlataforma * 3) {
								moviendoAdelante = false; // Cambiar dirección
							}
						}
						else {
							// Mover hacia el módulo 2 (posición original)
							nuevaPosicion.Y -= pasoMovimiento; // Mover un paso pequeño
							if (nuevaPosicion.Y <= posicionInicial.Y + anchoComponentePlataforma) {
								moviendoAdelante = true; // Cambiar dirección
							}
						}
						nuevaPlataforma->SetActorLocation(nuevaPosicion);
						}, 0.01f, true); // Intervalo ajustado a 0.01 segundos para hacerlo más lento y suave
				}
			}
				if (ncp < 10) {
					posicionInicial.Z = posicionInicial.Z + incrementoAltoComponentePlataforma;
				}
			
		}

		     posicionInicial.Z = posicionInicial.Z + incrementoAltoEntrePisos;
		     posicionInicial.Y = posicionInicial.Y + incrementoInicioPiso;
	}

	// Mostrar en pantalla las claves y las posiciones de las plataformas para debug
	for (const TPair<FString, AcomponentePlataforma*>& Entry : MapPlataformas) {
		FString clave = Entry.Key;
		AcomponentePlataforma* plataforma = Entry.Value;
		FVector posicion = plataforma->GetActorLocation();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Clave: %s, Posición: %s"), *clave, *posicion.ToString()));
	}

	// Crear el cono en la parte superior del módulo 5
	if (posicionUltimoModulo5 != FVector::ZeroVector) {
		FVector ConoLocation = FVector(posicionUltimoModulo5.X, posicionUltimoModulo5.Y, posicionUltimoModulo5.Z + 50.0f); // Ajustar Z según la altura real
		FRotator ConoRotation(0.0f, 0.0f, 0.0f);   // Rotación inicial del cono

		// Spawnear el Cono
		GetWorld()->SpawnActor<AConoDK>(AConoDK::StaticClass(), ConoLocation, ConoRotation);
	}

	GetWorld()->GetTimerManager().SetTimer(SpawnBarrilTimerHandle, this, &ADonkeyKong_L02GameMode::SpawnBarril, 3.0f, true);

	// Crear una esfera en la parte superior del primer piso que baje rebotando hasta llegar al suelo
	for (int nes = 0; nes < 5; nes++) {
		FTransform SpawnLocationEsfera;
		SpawnLocationEsfera.SetLocation(FVector(1300.0f, -200.0f, 1060.0f));
		SpawnLocationEsfera.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
		esferasMap.Add(nes, GetWorld()->SpawnActor<AEsfera>(AEsfera::StaticClass(), SpawnLocationEsfera));
	}
}

void ADonkeyKong_L02GameMode::SpawnBarril()
{
	if (contadorBarriles < numeroMaximoBarriles) {
		// Spawn de un objeto barril en la escena sobre la primera plataforma
		FTransform SpawnLocationBarril;
		SpawnLocationBarril.SetLocation(FVector(1160.0f, 900.0f, 860.0f));
		SpawnLocationBarril.SetRotation(FQuat(FRotator(90.0f, 0.0f, 0.0f)));
		barriles.Add(GetWorld()->SpawnActor<ABarril>(ABarril::StaticClass(), SpawnLocationBarril));
		contadorBarriles++;

		if (contadorBarriles >= numeroMaximoBarriles) {
			GetWorld()->GetTimerManager().ClearTimer(SpawnBarrilTimerHandle);
		}
	}
}
