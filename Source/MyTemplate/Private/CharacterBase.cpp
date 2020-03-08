// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "MyAttributeSetBase.h"

#include "..\Public\CharacterBase.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GrappleComponent.h"

#include "Components/SceneComponent.h"



// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(34.0f, 96.0f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;


	GetCharacterMovement()->bOrientRotationToMovement = true;	 //allows character to rotate in direction its moving
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 360.0f);
	GetCharacterMovement()->JumpZVelocity = 1200.0f;
	GetCharacterMovement()->AirControl = 1.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom!"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Followcam!"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = true;

	Dash = CreateDefaultSubobject<UArrowComponent>(TEXT("Dash!"));
	Dash->SetupAttachment(RootComponent);
	Dash->bHiddenInGame = true;
	//Dash->bVisible = true;
	Dash->RelativeLocation = FVector(100, 0, 0);


	VaultChecker= CreateDefaultSubobject<UBoxComponent>(TEXT("VAULTCHECKER!"));
	VaultChecker->SetupAttachment(RootComponent);
	VaultChecker->SetRelativeScale3D(FVector(2, 2, 2));
	VaultChecker->SetBoxExtent(FVector(32.f, 32.f, 32.f));

	Left = CreateDefaultSubobject<UBoxComponent>(TEXT("Left!"));
	Left->SetupAttachment(RootComponent);
	Left->SetRelativeScale3D(FVector(0.4f, 0.5f, 2.0f));
	Left->SetBoxExtent(FVector(30.f, 32.f, 32.f));
	Left->SetRelativeLocation(FVector(-19.999979f, -50, 30.f));
	Left->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.f));

	Right = CreateDefaultSubobject<UBoxComponent>(TEXT("Right!"));
	Right->SetupAttachment(RootComponent);
	Right->SetRelativeScale3D(FVector(0.4f, 0.5f, 2.0f));
	Right->SetBoxExtent(FVector(30.f, 32.f, 32.f));
	Right->SetRelativeLocation(FVector(-19.999979f, 50, 30.f));
	Right->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.f));

	Hit = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hit!"));
	Hit->SetupAttachment(RootComponent);
	Hit->SetRelativeScale3D(FVector(0.9f, 0.9f, 0.9f));
	Hit->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.f));

	Front = CreateDefaultSubobject<UBoxComponent>(TEXT("Front!"));
	Front->SetupAttachment(RootComponent);
	Front->SetRelativeScale3D(FVector(1.0f, 0.5f, 2.0f));
	Front->SetBoxExtent(FVector(30.f, 32.f, 32.f));
	Front->SetRelativeLocation(FVector(60.0f, 10, 30.f));
	Front->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.f));

	//CHRISTIAN
	GrappleComponent = CreateDefaultSubobject<UGrappleComponent>(TEXT("GrappleComponent"));
	
	GrappleHook = CreateDefaultSubobject<UCableComponent>(TEXT("GrapplingHook"));
	GrappleHook->SetupAttachment(RootComponent);


	ChargingTimelineInitiate = false;
	initiateRamParticles = 0;
	
	Stamina = 1.0f;
	VaultmechanicDoOnce = true;
	SlideDooNce = true;
	PlayGrappleSoundOnce = true;
	
	EndOfGame = false;




}


void ACharacterBase::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bfromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Vault"))
	{
		VaultCollision = true;
	}
}

void ACharacterBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Vault"))
	{
		VaultCollision = false;

	}

}



void ACharacterBase::OnOverlapEndForFrontBox(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	

	if (OtherActor->IsA(ACharacterBase::StaticClass()))
	{
		if (OtherActor != this)
		{
			OtherHitPlayer = Cast< ACharacterBase>(OtherActor);
			if (OtherHitPlayer != nullptr && OtherHitPlayer != this)
			{									//ADDED THIS CONDITION
				HitTheOtherPlayer = false;
			}
		}
	}	
	 //RAMING INTO OTHER PLAYER AND SEND THEM FLYING

	
	
}









// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	BaseLookUpRate = 45.0f;
	BaseTurnRate = 45.0f;
	VaultChecker->OnComponentBeginOverlap.AddDynamic(this, &ACharacterBase::OnBeginOverlap);
	VaultChecker->OnComponentEndOverlap.AddDynamic(this, &ACharacterBase::OnOverlapEnd);

	Front->OnComponentBeginOverlap.AddDynamic(this, &ACharacterBase::OnBeginOverlapForFrontBox);
	Front->OnComponentEndOverlap.AddDynamic(this, &ACharacterBase::OnOverlapEndForFrontBox);

	Meshlocation = GetMesh()->GetRelativeTransform().GetLocation();

	
	
	//SETTING THE SCALES OF THE LEFT WALL AND RIGHT WALL COLLIDER
	Left->SetRelativeScale3D(FVector(0.4f, 1.5f, 2.0f));
	//Left->SetRelativeLocation(FVector(0.000018, -64, 30));
	Right->SetRelativeScale3D(FVector(0.4f, 1.5f, 2.0f));
	//Right->SetRelativeLocation(FVector(0.000021, -64, 30));

	Leftwall_RaycastLengthChecker = -100.0f;
	Rightwall_RaycastLengthChecker = 100.0f;

	//starts with this bool so that once the delay function is hit it goes right into it, this is for the Leftwall_RaycastLengthChecker and Rightwall_RaycastLengthChecker
	ResettheRightRaycast = true;
	ResettheLeftRaycast = true;
	//walljump off velocity floats
	forward_WallJumpVel = 1000.0f;
	Side_WallJumpVel = 1000.0f;


	//for the ram timeline
	TimelineDuration = 0.0f;
	OtherHitPlayer = nullptr;
	RamEffects_TimelineInitiate = false;


	//CHANGE RIGHT AFTER GRAPPLES FIXED
	GrappleTimer = 2.5f;
	//StopTheWallrunRaycast = false;

	HookDistance = 2000.0f;
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
	StaminaBar();

	HorizontalVelocity();
	
	//SLIDING
	Vertical_Collision();		
	//Timeline like functions
	TimelineForSliding();

	//VAULTING TIMELINE
	TimelineForVaulting();
	TimelineForZoomingIn();
	TimelineForZoomingOut();
	TimelineForVaultingUp();


	//WALLRUNNING TIMELINE
	TimelineForWallRunning();
	//WALLRUNNING
	WallRunRaycast();
	if (GetMovementComponent()->IsFalling() == false)
	{
		Landed();
	}
	if (GetMovementComponent()->IsFalling() == true)
	{
		FVector VelocityVector = GetVelocity();
		CharacterVelocity = VelocityVector.Size();

		//if (CharacterVelocity >= 100)
		{
			WallRunner();

		}
	}

	//grapple 
	TimelineForGrapplePulling();

	//ram
	
	TimelineForCharging();


	if (OtherHitPlayer == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Your message"));

	}

	TimelineEndOfRamEffects();


}






// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	//if (EndOfGame == false)
	{

		
		Super::SetupPlayerInputComponent(PlayerInputComponent);
		PlayerInputComponent->BindAxis("LookSideways", this, &APawn::AddControllerYawInput);	//BINDS THE "TURN" action mappings in project settings to the "use controller yaw settings in the BP.
													//pawn									//since "TURN" is mouseX, so the mouse X will control the yaw input

		PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);  //BINDS THE "LookUp" action mappings in project settings to the "use controller pitch settings in the BP.
														//pawn								//since "LookUp" is mouseX, so the mouse Y will control the pitch input

	//THIS CALLS THE TWO MOVEMENT FUNCTIONS

				//CHECK THIS FUNCTION FOR UNDERSTANDING
		
		PlayerInputComponent->BindAxis("Moveforward", this, &ACharacterBase::Moveforward);
		PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::MoveRight);
		
		PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterBase::CharcterJump);
		PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacterBase::DontJump);

		PlayerInputComponent->BindAction("Slide", IE_Pressed, this, &ACharacterBase::Slide);
		PlayerInputComponent->BindAction("Slide", IE_Released, this, &ACharacterBase::DontSlide);
		
		//PlayerInputComponent->BindAction("Grapple", IE_Pressed, GrappleComponent, &UGrappleComponent::Grapple);
		PlayerInputComponent->BindAction("Grapple", IE_Pressed, this, &ACharacterBase::GrappleAbility);
		PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACharacterBase::Ram);
	}


}

void ACharacterBase::Moveforward(float axis)
{
	if (M_Hanging == false)
	{

		AddMovementInput(GetActorForwardVector() * axis);

	}

}

void ACharacterBase::MoveRight(float axis)
{

	if (M_Hanging == false)
	{
		AddMovementInput(GetActorRightVector() * axis);


	}
}

void ACharacterBase::HorizontalVelocity()
{
	//Characters HorizontalVelocity

	FVector VelocityVector = GetVelocity();
	CharacterVelocity = VelocityVector.Size();
	VaultVelocity = CharacterVelocity;


}





void ACharacterBase::StaminaBar()
{
	//because its being called every tick, refix it, its reset


	if (Stamina < 0)
	{
		Stamina = 0;

	}

	if (Stamina < 1)
	{
		Stamina = Stamina + 0.0009f;
	}


}





// SLIDE!!!!!!!!!!!!!!!!



void ACharacterBase::Slide()
{
	

	FHitResult Out1;
	FVector Start1 = GetActorLocation() + FVector(0, 0, 44);
	FVector End1 = Start1 + GetActorForwardVector() * 400;
	FCollisionQueryParams  CollisionP1;


	LeftShiftPressed = true;
	ColliderCheckerMod = 200;
	float VelocityVector = GetVelocity().Size();

	if (GetMovementComponent()->IsFalling() == false && VelocityVector >= 100)
	{

		float AnimationDuration = 0.6f;
		FastEnoughToSlide = true;

		
		PlayAnimMontage(IdleToSlide, AnimationDuration, NAME_None);

		SlideCollider();

		//delay
		GetWorld()->GetTimerManager().SetTimer(SlideTimer, this, &ACharacterBase::ResetTimer, AnimationDuration, false);

	}
	else
	{
		FastEnoughToSlide = false;
	}
	
}




void ACharacterBase::DontSlide()	//WHEN LIFE THE SLIDE SHIFT KEY
{
	
	LeftShiftPressed = false;
	
}




void ACharacterBase::ResetTimer()
{
	
	GetCapsuleComponent()->SetCapsuleSize(42.0f, 96.0f, true);

	//SUBJECT TO CHANGE!!!!!
	GetMesh()->SetRelativeLocation(FVector(Meshlocation.X, Meshlocation.Y, Meshlocation.Z), false, 0, ETeleportType::None);


	//RESET THE TIMER
	GetWorld()->GetTimerManager().ClearTimer(SlideTimer);
	
}







void ACharacterBase::Vertical_Collision()
{

	
	if (VerticalCollision == true)
	{

		//initiate the timeline
		SlidingTimelineInitiate = true;


	}


	if (VerticalCollision == false)
	{
		//STOP the timeline
		SlidingTimelineInitiate = false;


		GetCapsuleComponent()->SetCapsuleSize(42.0f, 96.0f, true);


		//SUBJECT TO CHANGE!!!!!

		if (GetMesh()->GetRelativeTransform().GetLocation() != Meshlocation)
		{
			GetMesh()->SetRelativeLocation(FVector(Meshlocation.X, Meshlocation.Y, Meshlocation.Z), false, 0, ETeleportType::None);
		}
	}
	

}




//MIGHT HAVE TO FIX THIS

void ACharacterBase::SlideColliderDoOnce()
{
	
	if (SlideDooNce == true)
	{
		VerticalCollision = false;
		AddMovementInput(Dash->GetForwardVector(), 1.0f, true);
		GetCapsuleComponent()->SetCapsuleSize(42, 96, true);
		//line below SUBJECT TO CHANGE!!!!!
		GetMesh()->SetRelativeLocation(FVector(Meshlocation.X - 60.0f, Meshlocation.Y, Meshlocation.Z + 60.0f), false, 0, ETeleportType::None);

		SlideDooNce = false;
	}
	
}



void ACharacterBase::ResetSlideColliderDoOnce()
{
	
	SlideDooNce = true;
	
}
//!!!!!!!!!!!!!!!!!!!!



void ACharacterBase::SlideCollider()
{
	
	TArray<AActor*> none;

	//horizontal raycast
	FHitResult Out1;
	FVector Start1 = GetActorLocation() + FVector(0, 0, 44);
	FVector End1 = Start1 + GetActorForwardVector() * 400;
	FCollisionQueryParams  CollisionP1;

	//DrawDebugLine(GetWorld(), Start1, End1, FColor::Red, false, 1, 0, 1);

	bool HorizontalCheckerIsHit = GetWorld()->LineTraceSingleByChannel(Out1, Start1, End1, ECC_Visibility, CollisionP1);



	if (HorizontalCheckerIsHit == true)
	{
		if (Out1.Actor->ActorHasTag("SLIDEDOWN") == true)
		{
			CollisionsForSliding = true;
		}
		else if (Out1.Actor->ActorHasTag("SLIDEDOWN") == false)
		{
			CollisionsForSliding = false;
		}
	}
	else if (HorizontalCheckerIsHit == false)
	{
		CollisionsForSliding = false;
	}


	//vertical raycast from the player
	FHitResult Out2;
	FVector Start2 = GetActorLocation() + FVector(0, 0, 44) + (GetActorForwardVector() * 50);
	FVector End2 = Start2 + GetActorUpVector() * 100;


	//DrawDebugLine(GetWorld(), Start2, End2, FColor::Red, false, 1, 0, 1);

	bool VerticalCheckerIsHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start2, End2, 40, TraceTypeQuery1, false, none, EDrawDebugTrace::None, Out2, true, FLinearColor::Green, FLinearColor::Red, 5);



	//vertical raycast behind the player
	FHitResult Out3;
	FVector Start3 = GetActorLocation() + GetActorForwardVector() * -100;
	FVector End3 = Start3 + GetActorUpVector() * 100;

	//DrawDebugLine(GetWorld(), Start3, End3, FColor::Red, false, 1, 0, 1);


	bool VerticalBehindCheckerIsHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start3, End3, 40, TraceTypeQuery1, false, none, EDrawDebugTrace::None, Out3, true, FLinearColor::Gray, FLinearColor::Red, 5);


	if (VerticalCheckerIsHit == true)
	{

		if (Out2.Actor->ActorHasTag("SLIDEDOWN") == true)
		{

			VerticalCollision = true;
			GetCapsuleComponent()->SetCapsuleSize(20.0f, 10.0f, true);

			//line below SUBJECT TO CHANGE!!!!!
			GetMesh()->SetRelativeLocation(FVector(Meshlocation.X - 60.0f, Meshlocation.Y, Meshlocation.Z + 60.0f), false, 0, ETeleportType::TeleportPhysics);
			if (MoveForwards == true)
			{
				//do once RESET
				ResetSlideColliderDoOnce();
			}
			if (MoveForwards == false)
			{
				AddMovementInput(Dash->GetForwardVector(), 1.0f, true);
				//do once RESET
				ResetSlideColliderDoOnce();
			}

		}
		else if (Out2.Actor->ActorHasTag("SLIDEDOWN") == false)
		{
		
		}
		
	}
	if (VerticalCheckerIsHit == false)
	{
		if (VerticalBehindCheckerIsHit == false)
		{

			SlideDooNce = true;
			//DO ONCE!!!!!!!!!!!!
			SlideColliderDoOnce();
		}

	}


	if (VerticalBehindCheckerIsHit == true)
	{
		if (Out3.Actor->ActorHasTag("SLIDEDOWN") == true)
		{
			VerticalCollision = true;
			GetCapsuleComponent()->SetCapsuleSize(20.0f, 10.0f, true);

			//line below SUBJECT TO CHANGE!!!!!
			GetMesh()->SetRelativeLocation(FVector(Meshlocation.X - 60.0f, Meshlocation.Y, Meshlocation.Z + 60.0f), false, 0, ETeleportType::TeleportPhysics);

			if (MoveForwards == true)
			{
				//do once RESET
				ResetSlideColliderDoOnce();
			}
			else if (MoveForwards == false)
			{
				AddMovementInput(Dash->GetForwardVector(), 1.0f, true);
				//do once RESET
				ResetSlideColliderDoOnce();
			}
		}
	}
	else if (VerticalBehindCheckerIsHit == false && VerticalCheckerIsHit == false)
	{
		SlideDooNce = true;
		//DO ONCE!!!!!!!!!!!!
		SlideColliderDoOnce();
	}

	

}


void ACharacterBase::SlideInitiator()
{
	
	TArray<AActor*> none;

	//horizontal raycast
	FHitResult Out;
	FVector Start = GetActorLocation() + FVector(0, 0, 44) + (GetActorForwardVector() * ColliderCheckerMod);
	FVector End = Start + GetActorUpVector() * 100;

	bool InitiatingCheckerIsHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 30, TraceTypeQuery1, false, none, EDrawDebugTrace::None, Out, true, FLinearColor::Green, FLinearColor::Red, 5);

	if (InitiatingCheckerIsHit == true)
	{

		if (Out.Actor->ActorHasTag("SLIDEDOWN") == false)
		{
			VerticalCollision = false;
			ColliderCheckerMod = -30;

		}
	}
	else if (InitiatingCheckerIsHit == false)
	{
		VerticalCollision = false;
		ColliderCheckerMod = -30;

	}
	
}



void ACharacterBase::TimelineForSliding()
{
	
	if (SlidingTimelineInitiate == true)
	{
		SlideCollider();
	}
	

}
//WALLRUN ABILITY








void ACharacterBase::ResetLeftRaycast()
{

	Leftwall_RaycastLengthChecker = -100.0f;


	//RESET THE TIMER
	GetWorld()->GetTimerManager().ClearTimer(LeftRaycastResetter);

	//re-enables the execution of the delay function once Leftwall_RaycastLengthChecker=0;
	ResettheLeftRaycast = true;
}

void ACharacterBase::ResetRightRaycast()
{


	Rightwall_RaycastLengthChecker = 100.0f;

	//RESET THE TIMER
	GetWorld()->GetTimerManager().ClearTimer(RightRaycastResetter);

	//re-enables the execution of the delay function once Rightwall_RaycastLengthChecker=0;
	ResettheRightRaycast = true;
}



void ACharacterBase::Landed()
{
	//GetCharacterMovement()->GravityScale = 3;
	LeftWall = false;
	RightWall = false;
	OnTheWall = false;
	//stop time line
	WallRunTimelineInitiate = false;

}


void ACharacterBase::WallRunner()
{

	if (CloseToTheWall == true)
	{
		OnTheWall = true;
		//initiate the wall run time line
		WallRunTimelineInitiate = true;
	}
	else if (CloseToTheWall == false)
	{
		OnTheWall = false;

	}

}

//HAVENT ADDED IN THE TIMELINE NOR PUT THIS IN THE TICK FUNCTION
void ACharacterBase::WallRunRaycast()
{
	//ONLY LETS THIS RAYCAST FUNCTION WORK IF NOT VAULTING !!!!!!!!!!!!!

	//if (StopTheWallrunRaycast == false)
	{
		TArray<AActor*> none;

		//left raycast
		FHitResult Out;
		FVector Start = GetActorLocation();
		FVector End = Start + (GetActorRightVector() * Leftwall_RaycastLengthChecker);
		FCollisionQueryParams  CollisionP;


		bool LeftChecker = GetWorld()->LineTraceSingleByChannel(Out, Start, End, ECC_Visibility, CollisionP);
		//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

		//right raycast
		FHitResult Out1;
		FVector Start1 = GetActorLocation();
		FVector End1 = Start1 + (GetActorRightVector() * Rightwall_RaycastLengthChecker);
		FCollisionQueryParams  CollisionP1;

		bool RightChecker = GetWorld()->LineTraceSingleByChannel(Out1, Start1, End1, ECC_Visibility, CollisionP1);
		//DrawDebugLine(GetWorld(), Start1, End1, FColor::Green, false, 1, 0, 1);
		if (LeftChecker == true)
		{
			if (Out.Actor->ActorHasTag("RUNWALL") == true)
			{
				GetCharacterMovement()->GravityScale = 0.3;
				CloseToTheWall = true;
				LeftWall = true;

			}
			if (Out.Actor->ActorHasTag("RUNWALL") == false)
			{
				GetCharacterMovement()->GravityScale = 3.0f;
				CloseToTheWall = false;
				LeftWall = false;
				OnTheWall = false;

				//stop time line
				WallRunTimelineInitiate = false;
			}

		}
		else if (LeftChecker == false)
		{
			if (Leftwall_RaycastLengthChecker == 0)
			{
				if (ResettheLeftRaycast == true)
				{

					//delay
					GetWorld()->GetTimerManager().SetTimer(LeftRaycastResetter, this, &ACharacterBase::ResetLeftRaycast, 0.5f, false);
					ResettheLeftRaycast = false;
				}
			}


			if (RightChecker == true)
			{
				if (Out1.Actor->ActorHasTag("RUNWALL") == true)
				{
					GetCharacterMovement()->GravityScale = 0.3;
					CloseToTheWall = true;
					RightWall = true;


				}
				if (Out1.Actor->ActorHasTag("RUNWALL") == false)
				{
					GetCharacterMovement()->GravityScale = 3;
					CloseToTheWall = false;
					RightWall = false;
					OnTheWall = false;
					//stop time line
					WallRunTimelineInitiate = false;
				}
			}
			if (RightChecker == false)
			{




				GetCharacterMovement()->GravityScale = 3;
				CloseToTheWall = false;
				RightWall = false;
				OnTheWall = false;
				//stop time line
				WallRunTimelineInitiate = false;



				if (Rightwall_RaycastLengthChecker == 0)
				{
					if (ResettheRightRaycast == true)
					{
						//delay
						GetWorld()->GetTimerManager().SetTimer(RightRaycastResetter, this, &ACharacterBase::ResetRightRaycast, 0.5f, false);
						ResettheRightRaycast = false;
					}
				}
			}
		}
	}
}

void ACharacterBase::TimelineForWallRunning()
{

	if (WallRunTimelineInitiate == true)
	{
		if (MoveForwards == true)
		{
			FVector ForwardVelocity = Dash->GetForwardVector() * 1000;
			FVector LaunchVelocity = FVector(ForwardVelocity.X, ForwardVelocity.Y, 0);
			LaunchCharacter(LaunchVelocity, true, true);

		}
		if (MoveForwards == false)
		{

			FVector LaunchVelocity = FVector(0, 0, -250);
			LaunchCharacter(LaunchVelocity, false, false);
			GetCharacterMovement()->GravityScale = 3;
		}
	}

}


//HERE!!!!!!!!!!!!
//OVERLAP FOR WALLRUNNING 
void ACharacterBase::OnBeginOverlapForFrontBox(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bfromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->ActorHasTag("RUNWALL") == false)
	{
		MoveForwards = false;





		if (CloseToTheWall == true)
		{
			if (LeftWall == true)
			{

				FVector LaunchUpwardsVelocity = FVector(0, 0, 1000);
				FVector JumpoffWallVeclocity = FVector((Dash->GetForwardVector().X * forward_WallJumpVel) + (Left->GetForwardVector().X * Side_WallJumpVel), (Dash->GetForwardVector().Y * forward_WallJumpVel) + (Left->GetForwardVector().Y * Side_WallJumpVel), 0);

				LaunchCharacter(LaunchUpwardsVelocity + JumpoffWallVeclocity, true, true);

				Leftwall_RaycastLengthChecker = 0;
				LeftWall = false;
				RightWall = false;
				CloseToTheWall = false;

				//stop time line
				WallRunTimelineInitiate = false;



			}
			if (LeftWall == false)
			{
				if (RightWall == true)
				{

					FVector LaunchUpwardsVelocity = FVector(0, 0, 1000);
					FVector JumpoffWallVeclocity = FVector((Dash->GetForwardVector().X * forward_WallJumpVel) + (Right->GetForwardVector().X * Side_WallJumpVel), (Dash->GetForwardVector().Y * forward_WallJumpVel) + (Right->GetForwardVector().Y * Side_WallJumpVel), 0);

					LaunchCharacter(LaunchUpwardsVelocity + JumpoffWallVeclocity, true, true);

					Rightwall_RaycastLengthChecker = 0;
					LeftWall = false;
					RightWall = false;
					CloseToTheWall = false;

					//stop time line
					WallRunTimelineInitiate = false;

				}
				if (RightWall == false)
				{
					LeftWall = false;
					RightWall = false;
					CloseToTheWall = false;

					//stop time line
					WallRunTimelineInitiate = false;



				}

			}
		}
	}


	//HITTING THE OTHERPLAYER TO SEND THEM FLYING

	if (OtherActor->IsA(ACharacterBase::StaticClass()))
	{
		if (OtherActor != this)
		{
			OtherHitPlayer = Cast< ACharacterBase>(OtherActor);
			if (OtherHitPlayer != nullptr &&  OtherHitPlayer != this)
			{
				HitTheOtherPlayer = true;
			}
		}
	}

	

	//RAMMING INTO WALLS
	if (RamUse == true)
	{




	}
}




//JUMP!!!!!!!!!!!!!!!
void ACharacterBase::CharcterJump()
{

	if (CloseToTheWall == true)
	{
		if (LeftWall == true)
		{

			FVector LaunchUpwardsVelocity = FVector(0, 0, 1000);
			FVector JumpoffWallVeclocity = FVector((Dash->GetForwardVector().X * forward_WallJumpVel) + (Left->GetForwardVector().X * Side_WallJumpVel), (Dash->GetForwardVector().Y * forward_WallJumpVel) + (Left->GetForwardVector().Y * Side_WallJumpVel), 0);

			LaunchCharacter(LaunchUpwardsVelocity + JumpoffWallVeclocity, true, true);

			Leftwall_RaycastLengthChecker = 0;
			LeftWall = false;
			RightWall = false;
			CloseToTheWall = false;

			//stop time line
			WallRunTimelineInitiate = false;



		}
		if (LeftWall == false)
		{
			if (RightWall == true)
			{

				FVector LaunchUpwardsVelocity = FVector(0, 0, 1000);
				FVector JumpoffWallVeclocity = FVector((Dash->GetForwardVector().X * forward_WallJumpVel) + (Right->GetForwardVector().X * Side_WallJumpVel), (Dash->GetForwardVector().Y * forward_WallJumpVel) + (Right->GetForwardVector().Y * Side_WallJumpVel), 0);

				LaunchCharacter(LaunchUpwardsVelocity + JumpoffWallVeclocity, true, true);

				Rightwall_RaycastLengthChecker = 0;
				LeftWall = false;
				RightWall = false;
				CloseToTheWall = false;

				//stop time line
				WallRunTimelineInitiate = false;

			}
			if (RightWall == false)
			{
				LeftWall = false;
				RightWall = false;
				CloseToTheWall = false;

				//stop time line
				WallRunTimelineInitiate = false;



			}

		}
	}
	if (CloseToTheWall == false)
	{
		if (VaultCollision == false)
		{
			Jump();
			OnTheWall = false;

			//stop time line
			WallRunTimelineInitiate = false;
		}


	}
	//START THE VAULT TIMELINE!!!!!!!!!!!!

	VaultTimelineInitiate = true;



}

void ACharacterBase::DontJump()
{

	StopJumping();

}


//Grapple Pull



void ACharacterBase::GrappleAbility()
{
	if (M_Hanging == false)
	{
		if (Stamina > 0.4f)
		{
			TArray<AActor*> none;
			//ForwardGrappleCheckerIsHit 
			FHitResult Out;
			FVector Start = GetActorLocation() + FollowCamera->GetForwardVector()*100 ;
			FVector End = Start + (FollowCamera->GetForwardVector()*HookDistance);
			FCollisionQueryParams  CollisionP;
			CollisionP.bTraceComplex = false;
			CollisionP.bReturnPhysicalMaterial = true;
		
			
			bool ForwardGrappleCheckerIsHit =UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 10, TraceTypeQuery2, false, none, EDrawDebugTrace::None, Out, true, FLinearColor::Red, FLinearColor::Green, 5);

			if (ForwardGrappleCheckerIsHit == true)
			{
				HookLocation = Out.Location;
				OtherGrappledCharacter =Cast< ACharacterBase>(Out.Actor);


				if (OtherGrappledCharacter != nullptr)
				{
					if (OtherGrappledCharacter != this)
					{
						HookLocation = OtherGrappledCharacter->GetMesh()->GetSocketLocation("Pelvis");

						ShootGrappleHook();

						PlayAnimMontage(GrappleAnim, 1.f, NAME_None);

						//Delay
						GetWorld()->GetTimerManager().SetTimer(GrappleDelayForPull, this, &ACharacterBase::GrappleDelayPullResetter, 0.55f, false);


					}
					else if (OtherGrappledCharacter == this)
					{
						//switch to the grapple to waypoint
						GrappleComponent->Grapple();
						

					}

				}
				if (OtherGrappledCharacter ==nullptr)
				{
					//switch to the grapple to waypoint
					GrappleComponent->Grapple();


				}
			}
			else if (ForwardGrappleCheckerIsHit == false)
			{
				GrappleComponent->Grapple();
			}
		}
	}

}

void ACharacterBase::TimelineForGrapplePulling()
{
	if (GrapplePullTimelineInitiate == true)
	{
		
		if (OtherGrappledCharacter->M_Hanging == true)
		{
			ResetGrapple();
		}
		if (OtherGrappledCharacter->M_Hanging == false)
		{
			FVector grappleVeloc = FVector(Dash->GetForwardVector().X*-2000.f, Dash->GetForwardVector().Y*-2000.f,1500.f);

			OtherGrappledCharacter->LaunchCharacter(grappleVeloc, true, true);

			ResetGrapple();
			GrapplePullTimelineInitiate = false;
		}
		
	}
}



void ACharacterBase::ResetGrapple()
{	
	ShowVisibilityOfGrappleHook = false;
	//CanGrappleHook = true;
	PlayGrappleSoundOnce = true;	
}

void ACharacterBase::ShootGrappleHook()
{
	Stamina -= 0.4f;
	//Delay
	GetWorld()->GetTimerManager().SetTimer(GrappleDelayForVisiblity, this, &ACharacterBase::GrappleVisibility, 0.3f, false);	
}




void ACharacterBase::GrappleVisibility()
{
	ShowVisibilityOfGrappleHook = true;
	DoOncePlayGrappleSound();
	
	//RESET THE TIMER
	GetWorld()->GetTimerManager().ClearTimer(GrappleDelayForVisiblity);
	
}

void ACharacterBase::DoOncePlayGrappleSound()
{
	if (PlayGrappleSoundOnce == true)
	{
		//play sound
		UWorld* WorldContextObject = GetWorld();
		UGameplayStatics::PlaySound2D(WorldContextObject, GrapplePullSound, 5.0f, 1.0f, 0, NULL, NULL);

		PlayGrappleSoundOnce = false;
	}
}


void ACharacterBase::GrappleDelayPullResetter()
{
	GrapplePullTimelineInitiate = true;

	//RESET THE TIMER
	GetWorld()->GetTimerManager().ClearTimer(GrappleDelayForPull);
	   
}




//Vaulting
void ACharacterBase::TimelineForVaulting()
{
	

	if (VaultTimelineInitiate == true)
	{
		//Horizontal_VaultChecker from the bottom straight forwards
		FHitResult Out;
		FVector Start = GetActorLocation() - FVector(0, 0, 44);
		FVector End = Start + (GetActorForwardVector() * 100);
		FCollisionQueryParams  CollisionP;

		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 1);

		bool Horizontal_VaultCheckerIsHit = GetWorld()->LineTraceSingleByChannel(Out, Start, End, ECC_Visibility, CollisionP);

		if (Horizontal_VaultCheckerIsHit == true)
		{
			if (Out.GetActor()->ActorHasTag("Vault") == true)
			{

				FVector WallLocation = Out.Location;
				FVector WallNormal = Out.Normal;


				//line from up to down 
				FHitResult Out1;
				FRotator rot_1 = UKismetMathLibrary::MakeRotFromX(WallNormal);
				FVector ForwardVec_1 = UKismetMathLibrary::GetForwardVector(rot_1);

				FVector Start1 = (ForwardVec_1 * -10) + WallLocation + FVector(0, 0, 200.0f);
				FVector End1 = Start1 + FVector(0, 0, -200.0f);
				FCollisionQueryParams  CollisionP1;

				//DrawDebugLine(GetWorld(), Start1, End1, FColor::Green, true, 1, 0, 1);

				bool WallHeightChecker_IsHit = GetWorld()->LineTraceSingleByChannel(Out1, Start1, End1, ECC_Visibility, CollisionP1);

				if (WallHeightChecker_IsHit == true)
				{
					FVector WallHeight = Out1.Location;

					/*
					if ((WallHeight - WallLocation).Z > 60.0f)
					{
						ShouldClimb = true;
					}
					else
					{
						ShouldClimb = false;
					}
					*/

					FHitResult Out2;
					FRotator rot_2 = UKismetMathLibrary::MakeRotFromX(WallNormal);
					FVector ForwardVec_2 = UKismetMathLibrary::GetForwardVector(rot_2);

					FVector Start2 = (ForwardVec_2 * -50) + WallLocation + FVector(0, 0, 250.0f);
					FVector End2 = Start2 + FVector(0, 0, -300.0f);
					FCollisionQueryParams  CollisionP2;

					//DrawDebugLine(GetWorld(), Start2, End2, FColor::Red, true, 1, 0, 1);


					bool WallThicknessChecker_IsHit = GetWorld()->LineTraceSingleByChannel(Out2, Start2, End2, ECC_Visibility, CollisionP2);

					if (WallThicknessChecker_IsHit == true)
					{
						FVector OtherWallHigh = Out2.Location;

						if ((WallHeight - OtherWallHigh).Z > 0)
						{
							WallThick = false;
						}
						else
						{
							WallThick = true;
						}


						//if (ShouldClimb == false)
						//{
							//StopTheWallrunRaycast = true;
							VaultingFunctionInTimeline();
							
						//}

					}
					if (WallThicknessChecker_IsHit == false)
					{
						WallThick = false;



						//if (ShouldClimb == false)
						//{
							//StopTheWallrunRaycast = true;
							VaultingFunctionInTimeline();
						//}

					}




				}

			}
			if (Out.GetActor()->ActorHasTag("Vault") == false)
			{
				//stop the vault timeline
				VaultTimelineInitiate = false;
			}
		}
		if (Horizontal_VaultCheckerIsHit == false)
		{
			//stop the vault timeline
			VaultTimelineInitiate = false;
		}


	}

	
}

void ACharacterBase::VaultingFunctionInTimeline()
{
	
	if (WallThick == false)
	{		
		//do once vault
		VaultDoOnce();
					   		 
	}

	if (WallThick == true)
	{
	//	StopTheWallrunRaycast = false;

	}
	
}

void ACharacterBase::VaultDoOnce()
{
	if (VaultmechanicDoOnce == true)
	{
		PlayAnimMontage(VaultAnim, 1.f, NAME_None);

		//starts zooming in the character timeline
		ZoomingInTimelineInitiate = true;	

		//first delay
		GetWorld()->GetTimerManager().SetTimer(FirstVaultTimer, this, &ACharacterBase::ResetFirstVaultTimer, 0.3f, false);
		
		//stop the doonce function from executing
		VaultmechanicDoOnce = false;
	}
}

void ACharacterBase::ResetVaultDoOnce()
{
	VaultmechanicDoOnce = true;

}


void ACharacterBase::ResetFirstVaultTimer()
{

	//stop the zoom in timeline
	ZoomingInTimelineInitiate = false;

	//start the second timeline
	ZoomingOutTimelineInitiate = true;

	//disable collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	//second delay
	GetWorld()->GetTimerManager().SetTimer(SecondVaultTimer, this, &ACharacterBase::ResetSecondVaultTimer, 0.35f, false);


	//start the last timeline
	VaultingUpTimelineInitiate = true;

	//RESET THE TIMER
	GetWorld()->GetTimerManager().ClearTimer(FirstVaultTimer);

}

void ACharacterBase::TimelineForZoomingIn()
{
	if (ZoomingInTimelineInitiate == true)
	{
		CameraBoom->TargetArmLength -= 5.0f;
	}
}

void ACharacterBase::TimelineForZoomingOut()
{
	if (ZoomingOutTimelineInitiate == true)
	{
		if (CameraBoom->TargetArmLength < 500.f)
		{
			CameraBoom->TargetArmLength += 1.f;
		}
		else if(CameraBoom->TargetArmLength >= 500.f)
		{
			ZoomingOutTimelineInitiate = false;
		}
	}

}

void ACharacterBase::TimelineForVaultingUp()
{
	if (VaultingUpTimelineInitiate == true)
	{
		
		FVector LaunchVeloc = Dash->GetForwardVector() * 20.0f;
		LaunchCharacter(FVector(LaunchVeloc.X, LaunchVeloc.Y, 20.0f), false, true);

	}
}


void ACharacterBase::ResetSecondVaultTimer()
{

	//last delay
	GetWorld()->GetTimerManager().SetTimer(ThirdVaultTimer, this, &ACharacterBase::ResetThirdVaultTimer, 0.25f, false);



	//final launch over the obstacle
	FVector FinalLaunchVeloc = Dash->GetForwardVector() * VaultVelocity*0.001f;
	LaunchCharacter(FVector(FinalLaunchVeloc.X, FinalLaunchVeloc.Y, 500.0f), false, false);

	//enabling collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	//supposed to disable should climb var but its useless

	//reset the do once function
	ResetVaultDoOnce();
	
	//stop the vault timeline
	VaultTimelineInitiate = false;

	//RESET THE TIMER
	GetWorld()->GetTimerManager().ClearTimer(SecondVaultTimer);
}

void ACharacterBase::ResetThirdVaultTimer()
{
	//stop the second timeline

	VaultingUpTimelineInitiate = false;


	//RESET THE TIMER
	GetWorld()->GetTimerManager().ClearTimer(ThirdVaultTimer);
}






//RAM
void ACharacterBase::Ram()
{
	if (M_Hanging == false)
	{
		if (Stamina > 0.3)
		{
			if (GetCharacterMovement()->IsFalling() == false)
			{
				float Ram_animationLength = 1.0f;

				Stamina = Stamina - 0.5f;
				PlayAnimMontage(RamAnim, Ram_animationLength, NAME_None);
				RamUse = true;

				//initiate the charging time line
				ChargingTimelineInitiate = true;

				TimelineDuration = 0;

				//delay
				GetWorld()->GetTimerManager().SetTimer(EndOfRamDelay, this, &ACharacterBase::ResetRamEndDelay, Ram_animationLength, false);

			}
		}
	}
}


void ACharacterBase::ResetRamEndDelay()
{
	RamUse = false;
	//STOP the charging time line
	ChargingTimelineInitiate = false;


	//RESET THE TIMER
	GetWorld()->GetTimerManager().ClearTimer(EndOfRamDelay);
}


void ACharacterBase::TimelineForCharging()
{
	if (ChargingTimelineInitiate == true)
	{
		FVector ForwardVelocity = Dash->GetForwardVector() * 1000;
		FVector LaunchVelocity = FVector(ForwardVelocity.X, ForwardVelocity.Y, -500);
		LaunchCharacter(LaunchVelocity, true, true);


		if (HitTheOtherPlayer == true)
		{

			FVector ForwardVelocity = Dash->GetForwardVector() * 3500;
			FVector LaunchVelocity = FVector(ForwardVelocity.X, ForwardVelocity.Y, 1000);

			if (OtherHitPlayer != nullptr && OtherHitPlayer != this)
			{

				//sends the other player flying
				OtherHitPlayer->LaunchCharacter(LaunchVelocity, true, true);

				//play sound
				UWorld* WorldContextObject = GetWorld();
				UGameplayStatics::PlaySound2D(WorldContextObject, RamSound, 1.0f, 1.0f, 0, NULL, NULL);

				//camera shake
				UGameplayStatics::PlayWorldCameraShake(WorldContextObject, CamShake, FollowCamera->GetComponentLocation(), 0, 100, 1.0f, false);

				//initiate the endofRam time line
				RamEffects_TimelineInitiate = true;

				
			}




			if (OtherHitPlayer != nullptr && OtherHitPlayer != this)
			{
				
				OtherHitPlayer->DisableInput(nullptr);
				
			}
			
		}
	}

}

void ACharacterBase::TimelineEndOfRamEffects()
{



	if (RamEffects_TimelineInitiate == true)
	{
		TimelineDuration += 0.005f;
		
		if (OtherHitPlayer != nullptr && OtherHitPlayer != this)
		{
			if (OtherHitPlayer->GetCharacterMovement()->IsFalling() == false)
			{

				
				initiateRamParticles++;
				//RamParticles(initiateRamParticles);
				FVector head = FVector(OtherHitPlayer->GetMesh()->GetSocketLocation("head").X, OtherHitPlayer->GetMesh()->GetSocketLocation("head").Y, OtherHitPlayer->GetMesh()->GetSocketLocation("head").Z + 20.0f);

				if (initiateRamParticles > 30)
				{
					FActorSpawnParameters param;
					AActor* particleSpawner = GetWorld()->SpawnActor<AActor>(StunFromRam, head, FRotator(0, 0, 0), param);


					particleSpawner->AttachToComponent(OtherHitPlayer->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ("head"));

					initiateRamParticles = 0;
					
					/*
					//added from condition beneath
					if (OtherHitPlayer != nullptr)
					{
						OtherHitPlayer->EnableInput(NULL);
					}

					TimelineDuration = 0;
					RamEffects_TimelineInitiate = false;
					*/
				}
					
				
			}
		}

		if (TimelineDuration >= 1)
		{
			//Enabling Input in other player
			
			
			
			
			if (OtherHitPlayer != nullptr)
			{
				OtherHitPlayer->EnableInput(nullptr);
			}

			TimelineDuration = 0;
			RamEffects_TimelineInitiate = false;
			
		}
	}
}


void ACharacterBase::RamParticles(float num)
{
	/*
	FVector head = FVector(OtherHitPlayer->GetMesh()->GetSocketLocation("head").X, OtherHitPlayer->GetMesh()->GetSocketLocation("head").Y, OtherHitPlayer->GetMesh()->GetSocketLocation("head").Z + 20.0f);

	if (initiateRamParticles > 30)
	{
		FActorSpawnParameters param;
		AActor* particleSpawner =GetWorld()->SpawnActor<AActor>(StunFromRam, head, FRotator(0, 0, 0), param);

		
		particleSpawner->AttachToComponent(OtherHitPlayer->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ("head"));
		
		initiateRamParticles = 0;

	}
	*/

}

