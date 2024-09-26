// Fill out your copyright notice in the Description page of Project Settings.


#include "componentePlataforma.h"

// Sets default values
AcomponentePlataforma::AcomponentePlataforma()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("	StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));

	meshPlataforma = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshPlataforma->SetStaticMesh(MeshAsset.Object);
	RootComponent = meshPlataforma;

	FVector NewScale(2.0f, 2.5f, 0.5f); // Cambia estos valores según tus necesidades
	SetActorScale3D(NewScale);

}

// Called when the game starts or when spawned
void AcomponentePlataforma::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AcomponentePlataforma::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

