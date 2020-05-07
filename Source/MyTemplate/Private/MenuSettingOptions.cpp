// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSettingOptions.h"

// Sets default values for this component's properties
UMenuSettingOptions::UMenuSettingOptions()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMenuSettingOptions::BeginPlay()
{
	Super::BeginPlay();



	if (EnableSounds == true)
	{
		volumeMultiplier = 1.0f;

	}
	else
	{
		volumeMultiplier = 0;


	}


	if (EnableCameraShake == true)
	{
		OuterRadius = 100.0f;

	}
	else
	{

		OuterRadius = 0;

	}
	// ...
	
}


// Called every frame
void UMenuSettingOptions::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);




	// ...
}

