// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupArtifacts.h"
#include "..\Public\PowerupArtifacts.h"

// Sets default values
APowerupArtifacts::APowerupArtifacts()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


	Particle = CreateDefaultSubobject<UParticleSystem>(TEXT("ParticleSystem"));


	Mesh= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent);


	UWorld* WorldContextObject = GetWorld();
	//UGameplayStatics::SpawnEmitterAtLocation(WorldContextObject, StunFromRam, OtherHitPlayer->GetMesh()->GetSocketLocation("End"), FRotator(0, 0, 0), FVector(5, 5, 5), false, EPSCPoolMethod::None);




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

void APowerupArtifacts::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bfromSweep, const FHitResult& SweepResult)
{


}

