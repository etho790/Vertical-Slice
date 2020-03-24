// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CharacterBase.h"
#include "SlideSupports.generated.h"

UCLASS()
class MYTEMPLATE_API ASlideSupports : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlideSupports();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UArrowComponent* Arrow;

	//COLLISION FUNCTION
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//COLLISION FUNCTIONFOR THE FRONT
	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bfromSweep, const FHitResult& SweepResult);


	UPROPERTY(EditInstanceOnly,  BlueprintReadWrite)
		float  BounceVelocity;

	UPROPERTY(EditInstanceOnly,  BlueprintReadWrite)
		float  ArrowForwardVelocity;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	ACharacterBase* Character;

private:
	void SlidingTimeline();
	bool SlideNow;

};
