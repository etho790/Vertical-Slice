// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"

#include "Animation/AnimInstance.h"
#include "Components/TimelineComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint//UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "CharacterBase.generated.h"



UCLASS()
class MYTEMPLATE_API ACharacterBase : public ACharacter
{
	
	GENERATED_BODY()

public:
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

	//COLLISION FUNCTIONFOR THE VAULT
	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bfromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* VaultAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* RamAnim;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sounds, meta = (AllowPrivateAccess = "true"))
		class USoundBase* RamSound;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CameraShake, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UCameraShake> CamShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Particles, meta = (AllowPrivateAccess = "true"))
		UClass* StunFromRam;

	//COLLISION FUNCTION
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//COLLISION FUNCTIONFOR THE FRONT
	UFUNCTION()
		void OnBeginOverlapForFrontBox(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bfromSweep, const FHitResult& SweepResult);


	//COLLISION FUNCTION
	UFUNCTION()
		void OnOverlapEndForFrontBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	void StaminaBar();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Meshlocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ColliderCheckerMod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Leftwall_RaycastLengthChecker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Rightwall_RaycastLengthChecker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool FastEnoughToSlide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector M_WallLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector M_WallNormal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector M_HeightLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_IsClimbingLedge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool UseControllerRotationYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_CanGrab;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_CanClimb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool RamUse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool HitTheOtherPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool HasPickedUpPowerup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool FlashBombPicked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool HomingMissilePicked;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ThirdPickup;

	//christian
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UGrappleComponent* GrappleComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	void ResetTimer();

	void ResetLeftRaycast();
	void ResetRightRaycast();
	void ResetVault();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Moveforward(float axis);
	void MoveRight(float axis);
	void HorizontalVelocity();

	//JUMP
	void CharcterJump();
	void DontJump();

	//SLIDE	
	void Slide();
	void DontSlide();
	void Vertical_Collision();
	void SlideColliderDoOnce();
	void ResetSlideColliderDoOnce();
	UFUNCTION(BlueprintCallable)
		void SlideCollider();
	UFUNCTION(BlueprintCallable)
		void SlideInitiator();
	void TimelineForSliding();
	FTimerHandle SlideTimer;

	//WALL RUNNING
	float forward_WallJumpVel;
	float Side_WallJumpVel;
	FTimerHandle LeftRaycastResetter;
	FTimerHandle RightRaycastResetter;
	void Landed();
	UFUNCTION(BlueprintCallable)
		void WallRunner();

	UFUNCTION(BlueprintCallable)
		void WallRunRaycast();
	void TimelineForWallRunning();

	//vault
	void TimelineForVaulting();
	void VaultingFunctionInTimeline();
	FTimerHandle VaultResetter;

	//Climb		
	/*
	void ForwardTracer();
	void HeightTracer();

	void GrabLedge();
	UFUNCTION(BlueprintCallable)
		void ExitLedge();
	void ClimbLedge();
	void GetStandingPoint();
	FTimerHandle ClimbUpDelay;
	FTimerHandle EnableInputDelay;
	void ResetClimbUpDelay();
	void ResetEnableInputDelay();
	*/
	//Ram -Inate Ability
	void Ram();
	void TimelineForCharging();
	void TimelineEndOfRamEffects();
	float TimelineDuration;
	FTimerHandle EndOfRamDelay;
	void ResetRamEndDelay();



	float initiateRamParticles;
	void RamParticles( float num);
	float GrappleTimer;
private:
	bool SlideDooNce;
	bool SlidingTimelineInitiate;
	bool WallRunTimelineInitiate;
	bool VaultTimelineInitiate;
	bool ChargingTimelineInitiate;
	bool RamEffects_TimelineInitiate;

	


	bool ResettheLeftRaycast;
	bool ResettheRightRaycast;

	ACharacterBase* OtherHitPlayer;
	
};
