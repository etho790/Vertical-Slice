// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupArtifacts.h"
#include "..\Public\PowerupArtifacts.h"



// Sets default values
APowerupArtifacts::APowerupArtifacts()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	Particle->SetupAttachment(RootComponent);

	Mesh= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Particle);

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetupAttachment(Particle);


	//UWorld* WorldContextObject = GetWorld();
	//UGameplayStatics::SpawnEmitterAttached(AttachedVfx, Mesh, NAME_None, FVector(0, 0, 0), FRotator(0, 0, 0), EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::None);

	StartingPosition = GetActorLocation();
	StartingRotation = UKismetMathLibrary::UKismetMathLibrary::Conv_RotatorToVector(GetActorRotation());

	Character = nullptr;
	PrimaryActorTick.bCanEverTick = true;

	ArtifactPickedUp = false;
	//artifact = FindObject<UEnum>(ANY_PACKAGE, TEXT("PickupType"), true);
}

// Called when the game starts or when spawned
void APowerupArtifacts::BeginPlay()
{
	Super::BeginPlay();
	//colliding with the pickup
	Collider->OnComponentBeginOverlap.AddDynamic(this, &APowerupArtifacts::OnBeginOverlap);

	InitialTimeUntilRespawns = TimeUntilRespawns;

	if (Artifact_Type == 1)
	{
		artifact = PickupType::Homing_Missile;
	}
	if (Artifact_Type == 2)
	{
		artifact = PickupType::Flash_Bomb;
	}
	if (Artifact_Type == 3)
	{
		artifact = PickupType::ThirdPickup;
	}


}

// Called every frame
void APowerupArtifacts::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StartingRotation.X += 0.001f;
	StartingRotation.Y += 0.002f;
	StartingRotation.Z += 0.005f;

	if (StartingRotation.X > 0.1f)
	{
		StartingRotation.X = 0;
	}
	if (StartingRotation.Y > 0.1f)
	{
		StartingRotation.Y = 0;
	}
	if (StartingRotation.Z > 0.1f)
	{
		StartingRotation.Z = 0;
	}
	FRotator rotation = FRotator(0.1f, 0.2f, 0.5f);


	AddActorLocalRotation(rotation, false, nullptr, ETeleportType::None);

	Respawner();
}



void APowerupArtifacts::Respawner()
{
	if (ArtifactPickedUp == true)
	{
		TimeUntilRespawns = TimeUntilRespawns - 1;
		if (TimeUntilRespawns <= 0)
		{

			UWorld* world = GetWorld();

			UWorld* WorldContextObject = GetWorld();
			UGameplayStatics::SpawnEmitterAtLocation(WorldContextObject, PickedUpVfx, Mesh->GetComponentLocation(), FRotator(0, 0, 0), FVector(5,5,5), true, EPSCPoolMethod::None);
			world->SpawnActor<APowerupArtifacts>(spawn, GetActorLocation(), GetActorRotation());


			K2_DestroyActor();

		}
	}


}


void APowerupArtifacts::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bfromSweep, const FHitResult& SweepResult)
{

	Character = Cast< ACharacterBase>(OtherActor);	
	if (!Character) { return; }


	if (artifact == PickupType::Homing_Missile)
	{
		if (Character->HasPickedUpPowerup == false)
		{
			Character->HomingMissilePicked = true;
			//emitter vfx
			UWorld* WorldContextObject = GetWorld();
			UGameplayStatics::SpawnEmitterAtLocation(WorldContextObject, PickedUpVfx, Mesh->GetComponentLocation(), FRotator(0, 0, 0), true);

			//play sound
			
			UGameplayStatics::PlaySound2D(WorldContextObject, DestructionSound, 1.0f, 1.0f, 0, NULL, NULL);

			//picked up artifact
			ArtifactPickedUp = true;

			if (Character->HasPickedUpPowerup == false)
			{
				Particle->SetVisibility(false, false);
				Mesh->SetStaticMesh(NULL);
			}

		}
	}
	else if(artifact == PickupType::Flash_Bomb)
	{
		if (Character->HasPickedUpPowerup == false)
		{
			Character->FlashBombPicked = true;
			//emitter vfx
			UWorld* WorldContextObject = GetWorld();
			UGameplayStatics::SpawnEmitterAtLocation(WorldContextObject, PickedUpVfx, Mesh->GetComponentLocation(), FRotator(0, 0, 0), true);

			//play sound
			
			UGameplayStatics::PlaySound2D(WorldContextObject, DestructionSound, 1.0f, 1.0f, 0, NULL, NULL);

			//picked up artifact
			ArtifactPickedUp = true;

			if (Character->HasPickedUpPowerup == false)
			{
				Particle->SetVisibility(false, false);
				Mesh->SetStaticMesh(NULL);
			}

		}



	}
	else if (artifact == PickupType::ThirdPickup)
	{
		if (Character->HasPickedUpPowerup == false)
		{
			Character->ThirdPickup = true;
			//emitter vfx
			UWorld* WorldContextObject = GetWorld();
			UGameplayStatics::SpawnEmitterAtLocation(WorldContextObject, PickedUpVfx, Mesh->GetComponentLocation(), FRotator(0, 0, 0), true);

			//play sound
			
			UGameplayStatics::PlaySound2D(WorldContextObject, DestructionSound, 1.0f, 1.0f, 0, NULL, NULL);

			//picked up artifact
			ArtifactPickedUp = true;

			if (Character->HasPickedUpPowerup == false)
			{
				Particle->SetVisibility(false, false);
				Mesh->SetStaticMesh(NULL);
			}

		}

	}
	
}
