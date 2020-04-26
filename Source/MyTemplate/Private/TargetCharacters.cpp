// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetCharacters.h"

// Sets default values
ATargetCharacters::ATargetCharacters()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATargetCharacters::BeginPlay()
{
	Super::BeginPlay();
	
	RespawnCheckPoint =GetActorLocation();
}

// Called every frame
void ATargetCharacters::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATargetCharacters::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

