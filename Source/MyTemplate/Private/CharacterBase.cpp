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
	


	Stamina = 1.0f;
	SlideDooNce = true;
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
	
	 OtherHitPlayer = Cast< ACharacterBase>(OtherActor);
	 //RAMING INTO OTHER PLAYER AND SEND THEM FLYING
	if (OtherHitPlayer != NULL)
	{
		
		HitTheOtherPlayer = false;

	}
	
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
	forward_WallJumpVel = 1500.0f;
	Side_WallJumpVel = 2000.0f;


	//for the ram timeline
	TimelineDuration = 0;
	OtherHitPlayer = nullptr;
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

		if (CharacterVelocity >= 100)
		{
			WallRunner();

		}
	}

	//Climbing
	ForwardTracer();
	HeightTracer();

	//ram
	
	TimelineForCharging();
	TimelineEndOfRamEffects();
}






// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	//if (EndOfGame == false)
	{

		//THIS DOES NOT WORK!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
		
		PlayerInputComponent->BindAction("Grapple", IE_Pressed, GrappleComponent, &UGrappleComponent::Grapple);

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
		FastEnoughToSlide = true;

		FVector ForwardVelocity = Dash->GetForwardVector() * 2000;
		FVector LaunchVelocity = FVector(ForwardVelocity.X, ForwardVelocity.Y, 0);
		LaunchCharacter(LaunchVelocity, true, false);

		PlayAnimMontage(IdleToSlide, 0.6f, NAME_None);

		SlideCollider();

		//delay
		GetWorld()->GetTimerManager().SetTimer(SlideTimer, this, &ACharacterBase::ResetTimer, 0.6f, false);

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
	//UE_LOG(LogTemp, Warning, TEXT("Your message"));

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
		GetMesh()->SetRelativeLocation(FVector(Meshlocation.X - 70.0f, Meshlocation.Y, Meshlocation.Z + 70.0f), false, 0, ETeleportType::None);

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




	bool VerticalCheckerIsHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start2, End2, 5, TraceTypeQuery1, false, none, EDrawDebugTrace::None, Out2, true, FLinearColor::Red, FLinearColor::Red, 5);



	//vertical raycast behind the player
	FHitResult Out3;
	FVector Start3 = GetActorLocation() + GetActorForwardVector() * -100;
	FVector End3 = Start3 + GetActorUpVector() * 100;




	bool VerticalBehindCheckerIsHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start3, End3, 5, TraceTypeQuery1, false, none, EDrawDebugTrace::None, Out3, true, FLinearColor::Red, FLinearColor::Red, 5);


	if (VerticalCheckerIsHit == true)
	{

		if (Out2.Actor->ActorHasTag("SLIDEDOWN") == true)
		{

			VerticalCollision = true;
			GetCapsuleComponent()->SetCapsuleSize(20.0f, 10.0f, true);

			//line below SUBJECT TO CHANGE!!!!!
			GetMesh()->SetRelativeLocation(FVector(Meshlocation.X - 70.0f, Meshlocation.Y, Meshlocation.Z + 70.0f), false, 0, ETeleportType::TeleportPhysics);
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
			/*
				if (Out3.Actor->ActorHasTag("SLIDEDOWN") == true)
				{
					VerticalCollision = true;
					GetCapsuleComponent()->SetCapsuleSize(20.0f, 10.0f, true);

					//line below SUBJECT TO CHANGE!!!!!
					GetMesh()->SetRelativeLocation(FVector(Meshlocation.X - 70.0f, Meshlocation.Y, Meshlocation.Z + 70.0f), false, 0, ETeleportType::TeleportPhysics);

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
			*/
		}

	}
	if (VerticalCheckerIsHit == false)
	{
		if (VerticalBehindCheckerIsHit == false)
		{

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
			GetMesh()->SetRelativeLocation(FVector(Meshlocation.X - 70.0f, Meshlocation.Y, Meshlocation.Z + 70.0f), false, 0, ETeleportType::TeleportPhysics);

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

	bool InitiatingCheckerIsHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 20, TraceTypeQuery1, false, none, EDrawDebugTrace::None, Out, true, FLinearColor::Red, FLinearColor::Red, 5);

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
	GetCharacterMovement()->GravityScale = 3;
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
	OtherHitPlayer = Cast< ACharacterBase>(OtherActor);

	if (OtherHitPlayer != NULL)
	{

		HitTheOtherPlayer = true;

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

//Vaulting
void ACharacterBase::TimelineForVaulting()
{




	if (VaultTimelineInitiate == true)
	{
		//Horizontal_VaultChecker from the bottom straight forwards
		FHitResult Out;
		FVector Start = GetActorLocation() - FVector(0, 0, 44);
		FVector End = Start + (GetActorForwardVector() * 200);
		FCollisionQueryParams  CollisionP;

		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 1);

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

				DrawDebugLine(GetWorld(), Start1, End1, FColor::Red, false, 1, 0, 1);

				bool WallHeightChecker_IsHit = GetWorld()->LineTraceSingleByChannel(Out1, Start1, End1, ECC_Visibility, CollisionP1);

				if (WallHeightChecker_IsHit == true)
				{
					FVector WallHeight = Out1.Location;

					if ((WallHeight - WallLocation).Z > 60.0f)
					{
						ShouldClimb = true;
					}
					else
					{
						ShouldClimb = false;
					}


					FHitResult Out2;
					FRotator rot_2 = UKismetMathLibrary::MakeRotFromX(WallNormal);
					FVector ForwardVec_2 = UKismetMathLibrary::GetForwardVector(rot_2);

					FVector Start2 = (ForwardVec_2 * -50) + WallLocation + FVector(0, 0, 250.0f);
					FVector End2 = Start2 + FVector(0, 0, -300.0f);
					FCollisionQueryParams  CollisionP2;

					DrawDebugLine(GetWorld(), Start2, End2, FColor::Red, false, 1, 0, 1);


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


						if (ShouldClimb == false)
						{
							VaultingFunctionInTimeline();
						}

					}
					if (WallThicknessChecker_IsHit == false)
					{
						WallThick = false;



						if (ShouldClimb == false)
						{
							VaultingFunctionInTimeline();
						}

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
		GetCharacterMovement()->GravityScale = 0;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		PlayAnimMontage(VaultAnim, 1.5f, NAME_None);
		GetWorld()->GetTimerManager().SetTimer(VaultResetter, this, &ACharacterBase::ResetVault, 0.2f, false);

	}

}


void ACharacterBase::ResetVault()
{
	FVector LaunchVeloc = Dash->GetForwardVector() * VaultVelocity;
	LaunchCharacter(FVector(LaunchVeloc.X, LaunchVeloc.Y, 800), true, true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	ShouldClimb = false;
	GetCharacterMovement()->GravityScale = 3.0f;
	VaultTimelineInitiate = false;
	//RESET THE TIMER
	GetWorld()->GetTimerManager().ClearTimer(VaultResetter);
	//stop the vault timeline
	VaultTimelineInitiate = false;
}




//Climbing

void ACharacterBase::ForwardTracer()
{
	TArray<AActor*> none;
	FHitResult Out;
	FVector Start = GetActorLocation();
	FVector ForwardVec = UKismetMathLibrary::GetForwardVector(GetActorRotation());
	FVector End = FVector(ForwardVec.X * 150, ForwardVec.Y * 150, ForwardVec.Z) + Start;

	//ask if TraceTypeQuery3 is a custom trace channel
	bool HorizontalLineCheckerIsHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 20, TraceTypeQuery3, false, none, EDrawDebugTrace::None, Out, true, FLinearColor::Red, FLinearColor::Red, 5);

	if (HorizontalLineCheckerIsHit == true)
	{
		M_WallLocation = Out.Location;
		M_WallNormal = Out.Normal;
	}
}


void ACharacterBase::HeightTracer()
{
	TArray<AActor*> none;
	FHitResult Out;
	FVector Start = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 500) + (UKismetMathLibrary::GetForwardVector(GetActorRotation()) * 70);

	FVector End = FVector(Start.X, Start.Y, Start.Z - 500);

	//ask if TraceTypeQuery3 is a custom trace channel
	bool VerticalLineCheckerIsHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 20, TraceTypeQuery3, false, none, EDrawDebugTrace::None, Out, true, FLinearColor::Red, FLinearColor::Red, 5);

	if (VerticalLineCheckerIsHit == true)
	{
		M_HeightLocation = Out.Location;

		float val = GetMesh()->GetSocketLocation("PelvisSocket").Z - M_HeightLocation.Z;
		float M_ClimbHeightRange = -50;
		if (UKismetMathLibrary::InRange_FloatFloat(val, M_ClimbHeightRange, 0, true, true) == true)
		{
			if (M_IsClimbingLedge == false)
			{
				GrabLedge();
			}
		}
	}
}

void ACharacterBase::GrabLedge()
{
	UseControllerRotationYaw = false;
	GetCharacterMovement()->StopMovementImmediately();
	M_CanGrab = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	M_Hanging = true;


	float relativeLocationX = (UKismetMathLibrary::Multiply_VectorVector(M_WallNormal, FVector(22, 22, 0))).X + M_WallLocation.X;
	float relativeLocationY = M_WallLocation.Y + (UKismetMathLibrary::Multiply_VectorVector(M_WallNormal, FVector(22, 22, 0))).Y;
	float relativeLocationZ = M_HeightLocation.Z - 120.0f;


	FRotator Rotation = UKismetMathLibrary::Conv_VectorToRotator(M_WallNormal);

	FRotator RelativeRotation = FRotator(Rotation.Roll, 0, Rotation.Yaw - 180);


	//SETTING THE LOCATION AND ROTATION OF THE CHARACTER WHILE CLIMBING
	//SetActorRelativeRotation(RelativeRotation,false, NULL, ETeleportType:: None);
	SetActorRelativeLocation(FVector(relativeLocationX, relativeLocationY, relativeLocationZ), false, NULL, ETeleportType::None);

	GetCharacterMovement()->StopMovementImmediately();

	GetWorld()->GetTimerManager().SetTimer(ClimbUpDelay, this, &ACharacterBase::ResetClimbUpDelay, 0.1f, false);

}


void ACharacterBase::ResetClimbUpDelay()
{
	DisableInput(NULL);
	ClimbLedge();
	GetWorld()->GetTimerManager().SetTimer(EnableInputDelay, this, &ACharacterBase::ResetEnableInputDelay, 1.0f, false);

	//RESET THE TIMER
	GetWorld()->GetTimerManager().ClearTimer(ClimbUpDelay);
}

void ACharacterBase::ClimbLedge()
{
	if (M_IsClimbingLedge == false)
	{
		M_IsClimbingLedge = true;
		M_CanClimb = true;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		M_Hanging = false;

	}
}

void ACharacterBase::ResetEnableInputDelay()
{
	EnableInput(NULL);
	//RESET THE TIMER
	GetWorld()->GetTimerManager().ClearTimer(EnableInputDelay);
}



void ACharacterBase::ExitLedge()
{
	M_IsClimbingLedge = false;
	M_CanClimb = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

}


void ACharacterBase::GetStandingPoint()
{
	FRotator RelRotation = FRotator(GetCapsuleComponent()->K2_GetComponentRotation().Roll, 0, GetCapsuleComponent()->K2_GetComponentRotation().Yaw);

	GetCapsuleComponent()->SetRelativeRotation(RelRotation, false, NULL, ETeleportType::None);

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

				Stamina = Stamina - 0.3f;
				PlayAnimMontage(RamAnim, Ram_animationLength, NAME_None);
				RamUse = true;

				//initiate the charging time line
				ChargingTimelineInitiate = true;

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
			}




			if (OtherHitPlayer != nullptr && OtherHitPlayer != this)
			{
				OtherHitPlayer->DisableInput(NULL);
			}
			//initiate the endofRam time line
			RamEffects_TimelineInitiate = true;
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
				float delay = 0.5f;
				//delay
				GetWorld()->GetTimerManager().SetTimer(RamParticlesDelay, this, &ACharacterBase::ResetRamParticlesDelay, delay, false);

			}
		}

		if (TimelineDuration >= 5)
		{
			//Enabling Input in other player
			if (OtherHitPlayer != nullptr)
			{
				OtherHitPlayer->EnableInput(NULL);
			}

			TimelineDuration = 0;
			RamEffects_TimelineInitiate = false;
		}
	}
}



void ACharacterBase::ResetRamParticlesDelay()
{
	UWorld* WorldContextObject = GetWorld();
	UGameplayStatics::SpawnEmitterAtLocation(WorldContextObject, StunFromRam, OtherHitPlayer->GetMesh()->GetSocketLocation("End"), FRotator(0, 0, 0), FVector(5, 5, 5), false, EPSCPoolMethod::None);

	//USceneComponent* scenecomp;
	//UGameplayStatics::SpawnEmitterAttached(StunFromRam, scenecomp, OtherHitPlayer->GetMesh()->GetSocketByName("End"), OtherHitPlayer->GetMesh()->GetSocketLocation("End"), FRotator(0, 0, 0), EAttachLocation::SnapToTarget, true, EPSCPoolMethod::None);


	//RESET THE TIMER
	GetWorld()->GetTimerManager().ClearTimer(RamParticlesDelay);
}

