// Fill out your copyright notice in the Description page of Project Settings.


#include "MUI_Pawn.h"

// Sets default values
AMUI_Pawn::AMUI_Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMUI_Pawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMUI_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMUI_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

