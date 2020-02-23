// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
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


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UParticleSystem* Particle;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent * Collider;
};
