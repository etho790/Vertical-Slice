// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "CharacterBase.h"
#include "TypeOfArtifact.h"

#include "PowerupArtifacts.generated.h"





UCLASS()
class MYTEMPLATE_API APowerupArtifacts : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerupArtifacts();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector StartingRotation;
	FVector StartingPosition;
	FVector TimerTick;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Particles)
		UParticleSystemComponent* Particle;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent * Collider;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Particles, meta = (AllowPrivateAccess = "true"))
	//	UParticleSystem* AttachedVfx;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Particles, meta = (AllowPrivateAccess = "true"))
		UParticleSystem* PickedUpVfx;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sounds, meta = (AllowPrivateAccess = "true"))
		class USoundBase* DestructionSound;
	

	//MenuOptions
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UMenuSettingOptions* MenuOption;

	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = TypeofArtifact, meta = (AllowPrivateAccess = "true"))
		int Artifact_Type;

		PickupType artifact;
	
	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bfromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TimeUntilRespawns;
	float InitialTimeUntilRespawns;
	void Respawner();

	UPROPERTY(EditAnywhere)
		TSubclassOf< APowerupArtifacts> spawn;


	bool ArtifactPickedUp;
	ACharacterBase* Character;
};
