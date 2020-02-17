// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "MyAttributeSetBase.h"

#include "..\Public\CharacterBase.h"




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
	//EMPTY



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
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	StaminaBar();
	

	HorizontalVelocity();
	Vertical_Collision();//for sliding	
	SlideInitiator();


	//Timeline like functions
	TimelineForSliding();
	TimelineForWallRunning();



	//vaulting
	TimelineForVaulting();


	//for wall running
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
	ColliderCheckerMod = 150;
	float VelocityVector = GetVelocity().Size();
	
	if (GetMovementComponent()->IsFalling() == false && VelocityVector >= 100)
	{

		
		FVector ForwardVelocity = Dash->GetForwardVector() * 2000;
		FVector LaunchVelocity = FVector(ForwardVelocity.X, ForwardVelocity.Y, 0);
		LaunchCharacter(LaunchVelocity, true, false);
						
			PlayAnimMontage(IdleToSlide, 0.8f, NAME_None);

			SlideCollider();

			//delay
			GetWorld()->GetTimerManager().SetTimer(SlideTimer, this, &ACharacterBase::ResetTimer, 0.8f, false);

	}
	

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

void ACharacterBase::DontSlide()	//WHEN LIFE THE SLIDE SHIFT KEY
{
	
	LeftShiftPressed = false;	
	
}

void ACharacterBase::Vertical_Collision()
{
	
	if (VerticalCollision == false)
	{
		GetCapsuleComponent()->SetCapsuleSize(42.0f, 96.0f, true);


		//SUBJECT TO CHANGE!!!!!

		//if (GetMesh()->GetRelativeTransform().GetLocation() != Meshlocation)
		{
			GetMesh()->SetRelativeLocation(FVector(Meshlocation.X, Meshlocation.Y, Meshlocation.Z), false, 0, ETeleportType::None);
		}
	}
	
}






void ACharacterBase::SlideColliderDoOnce()
{
	
	if (SlideDooNce == true)
	{
		VerticalCollision = true;
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
		else if(Out1.Actor->ActorHasTag("SLIDEDOWN") == false)
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
	FVector Start2 = GetActorLocation() + FVector(0, 0, 44) + GetActorForwardVector()*50;
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
			GetMesh()->SetRelativeLocation(FVector(Meshlocation.X-70.0f, Meshlocation.Y, Meshlocation.Z+70.0f), false, 0, ETeleportType::TeleportPhysics);
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
	FVector Start = GetActorLocation() + FVector(0, 0, 44) + (GetActorForwardVector()*ColliderCheckerMod);
	FVector End = Start + GetActorUpVector() * 100;

	bool InitiatingCheckerIsHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 5, TraceTypeQuery1, false, none, EDrawDebugTrace::None, Out, true, FLinearColor::Red, FLinearColor::Red, 5);

	if (InitiatingCheckerIsHit == true)
	{
		if (Out.Actor->ActorHasTag("SLIDEDOWN") == true)
		{

			//CurveTimeline.PlayFromStart();
			SlidingTimelineInitiate = true;
		}
		else if (Out.Actor->ActorHasTag("SLIDEDOWN") == false)
		{
			VerticalCollision = false;
			ColliderCheckerMod = -30;
			//CurveTimeline.Stop();
			SlidingTimelineInitiate = false;
		}
	}
	else if (InitiatingCheckerIsHit == false)
	{
		VerticalCollision = false;
		ColliderCheckerMod = -30;
		//CurveTimeline.Stop();
		SlidingTimelineInitiate = false;
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
	FVector End = Start + (GetActorRightVector() * -60);
	FCollisionQueryParams  CollisionP;
	

	bool LeftChecker =GetWorld()->LineTraceSingleByChannel(Out, Start, End, ECC_Visibility, CollisionP);
	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	//right raycast
	FHitResult Out1;
	FVector Start1 = GetActorLocation();
	FVector End1 = Start1 + (GetActorRightVector() * 60);
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

		}
	}

}


//OVERLAP FOR WALLRUNNING 
void ACharacterBase::OnBeginOverlapForFrontBox(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 otherBodyIndex, bool bfromSweep, const FHitResult & SweepResult)
{

	if (OtherActor->ActorHasTag("RUNWALL") == false)
	{
		MoveForwards = false;
		if (CloseToTheWall == true)
		{
			if (LeftWall == true)
			{

				FVector LaunchUpwardsVelocity = FVector(0, 0, 1000);
				FVector JumpoffWallVeclocity = FVector((Dash->GetForwardVector().X * 1000) +(Left->GetForwardVector().X * 1850), (Dash->GetForwardVector().Y * 1000) +(Left->GetForwardVector().Y * 1850), 0);

				LaunchCharacter(LaunchUpwardsVelocity + JumpoffWallVeclocity, true, true);

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
					FVector JumpoffWallVeclocity = FVector((Dash->GetForwardVector().X * 1000) + (Right->GetForwardVector().X * 1850), (Dash->GetForwardVector().Y * 1000) + (Right->GetForwardVector().Y * 1850), 0);

					LaunchCharacter(LaunchUpwardsVelocity + JumpoffWallVeclocity, true, true);

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


}


//JUMP!!!!!!!!!!!!!!!
void ACharacterBase::CharcterJump()
{
	if (CloseToTheWall == true)
	{
		if (LeftWall == true)
		{

			FVector LaunchUpwardsVelocity = FVector(0, 0, 1000);
			
			FVector JumpoffWallVeclocity = FVector((Dash->GetForwardVector().X * 1000) + (Left->GetForwardVector().X * 1850), (Dash->GetForwardVector().Y * 1000) + (Left->GetForwardVector().Y * 1850), 0);

			LaunchCharacter(LaunchUpwardsVelocity + JumpoffWallVeclocity, true, true);

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
				FVector JumpoffWallVeclocity = FVector((Dash->GetForwardVector().X * 1000) + (Right->GetForwardVector().X * 1850), (Dash->GetForwardVector().Y * 1000) + (Right->GetForwardVector().Y * 1850), 0);

				LaunchCharacter(LaunchUpwardsVelocity + JumpoffWallVeclocity, true, true);

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
	//starting the vaulting time line
	VaultTimelineInitiate = true;



}

void ACharacterBase::DontJump()
{

	StopJumping();

}

//Vaulting
void ACharacterBase::TimelineForVaulting()
{
	//const TArray<FCollisionObjectQueryParams> none = { ECC_WorldStatic, ECC_Destructible };
	FHitResult Out;
	FVector Start = GetActorLocation() - FVector(0,0,44);
	FVector End = Start + (GetActorForwardVector() * 200);
	FCollisionQueryParams  CollisionP;


	
	//GetWorld()->LineTraceSingleByObjectType(Out,Start, End, none,)

}