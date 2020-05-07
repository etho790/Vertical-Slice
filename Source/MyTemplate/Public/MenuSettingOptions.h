// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MenuSettingOptions.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTEMPLATE_API UMenuSettingOptions : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMenuSettingOptions();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sounds, meta = (AllowPrivateAccess = "true"))
		bool EnableSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sounds, meta = (AllowPrivateAccess = "true"))
		float volumeMultiplier;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sounds, meta = (AllowPrivateAccess = "true"))
		bool EnableCameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CameraShake, meta = (AllowPrivateAccess = "true"))
		float OuterRadius;


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
