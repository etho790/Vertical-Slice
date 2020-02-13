// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"

#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint//UserWidget.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"

#include "CharacterBase.generated.h"



UCLASS()
class MYTEMPLATE_API ACharacterBase : public ACharacter
{
	
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UArrowComponent* Dash;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* VaultChecker;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* Right;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* Front;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Hit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TSubclassOf<UUserWidget> Player_UICLASS;



	
	bool EndOfGame;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_Hanging;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool MoveForwards;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool VaultCollision;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool OnTheWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CloseToTheWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool LeftWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool RightWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ShouldClimb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool WallThick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CollisionsForSliding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool LeftShiftPressed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float  Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float  CharacterVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float  VaultVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float  BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float  BaseTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool VerticalCollision;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* IdleToSlide;

	//COLLISION FUNCTION
	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bfromSweep, const FHitResult& SweepResult);


	//COLLISION FUNCTION
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	void StaminaBar();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Meshlocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ColliderCheckerMod;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	void ResetTimer();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void Moveforward(float axis);

	void MoveRight(float axis);
	
	void HorizontalVelocity();

	FTimerHandle SlideTimer;			//FTimerHandle has to be local for the timer to work but cant reset it


	void Slide();
	void DontSlide();
	void Vertical_Collision();


	void SlideColliderDoOnce();
	void ResetSlideColliderDoOnce();
	
	void SlideCollider();

private:
	
};
