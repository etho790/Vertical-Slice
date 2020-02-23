// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupArtifacts.h"

// Sets default values
APowerupArtifacts::APowerupArtifacts()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


	Particle = CreateDefaultSubobject<UParticleSystem>(TEXT("ParticleSystem"));


	Mesh= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent);


	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APowerupArtifacts::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerupArtifacts::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

