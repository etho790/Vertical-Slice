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

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	BaseLookUpRate = 45.0f;
	BaseTurnRate = 45.0f;
	VaultChecker->OnComponentBeginOverlap.AddDynamic(this, &ACharacterBase::OnBeginOverlap);
	VaultChecker->OnComponentEndOverlap.AddDynamic(this, &ACharacterBase::OnOverlapEnd);

	Meshlocation = GetMesh()->GetRelativeTransform().GetLocation();





}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	StaminaBar();
	

	HorizontalVelocity();
	Vertical_Collision();//for sliding
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
	/*
	LeftShiftPressed = true;
	ColliderCheckerMod = 150;
	float VelocityVector = GetVelocity().Size();
	
	if (GetMovementComponent()->IsFalling() == false && VelocityVector >= 100)
	{

		
		FVector ForwardVelocity = Dash->GetForwardVector() * 1500;
		FVector LaunchVelocity = FVector(ForwardVelocity.X, ForwardVelocity.Y, 0);
		LaunchCharacter(LaunchVelocity, true, false);
		if (CollisionsForSliding == true)
		{
			GetCapsuleComponent()->SetCapsuleSize(20.0f, 10.0f, true);
			GetMesh()->SetRelativeLocation(FVector(Meshlocation.X-70.0f, Meshlocation.Y, Meshlocation.Z + 85.0f), false, 0, ETeleportType::TeleportPhysics);
		
			PlayAnimMontage(IdleToSlide, 0.8f, NAME_None);

			SlideCollider();

			//delay
			GetWorld()->GetTimerManager().SetTimer(SlideTimer, this, &ACharacterBase::ResetTimer, 0.8f, false);

		}
		else if (CollisionsForSliding == false)
		{
			PlayAnimMontage(IdleToSlide, 0.8f, NAME_None);

			SlideCollider();

			//delay
			GetWorld()->GetTimerManager().SetTimer(SlideTimer, this, &ACharacterBase::ResetTimer, 0.8f, false);
		}

		
	}
	*/

}

void ACharacterBase::ResetTimer()
{
	GetCapsuleComponent()->SetCapsuleSize(42.0f, 96.0f, true);
	GetMesh()->SetRelativeLocation(FVector(Meshlocation.X, Meshlocation.Y, Meshlocation.Z), false, 0, ETeleportType::None);
	//UE_LOG(LogTemp, Warning, TEXT("Your message"));
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
		if (GetMesh()->GetRelativeTransform().GetLocation() != Meshlocation)
		{
			GetMesh()->SetRelativeLocation(FVector(Meshlocation.X, Meshlocation.Y, Meshlocation.Z), false, 0, ETeleportType::None);
		}
	}

}





//these bottom two are irrelevant, just get a boolean, that switches but its set to true at the start
void ACharacterBase::SlideColliderDoOnce()
{


}

void ACharacterBase::ResetSlideColliderDoOnce()
{


}

void ACharacterBase::SlideCollider()
{
	/*
	FHitResult Out1;
	FVector Start1 = GetActorLocation() + FVector(0, 0, 44);	
	FVector End1 = Start1 + GetActorForwardVector() * 400;
	FCollisionQueryParams  CollisionP1;

	DrawDebugLine(GetWorld(), Start1, End1, FColor::Red, false, 1, 0, 1);

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


	FHitResult Out2;
	FVector Start2 = GetActorLocation() + FVector(0, 0, 44) + GetActorForwardVector()*50;
	FVector End2 = Start2 + GetActorUpVector() * 100;
	FCollisionQueryParams  CollisionP2;

	DrawDebugLine(GetWorld(), Start2, End2, FColor::Blue, false, 1, 0, 1);

	bool VerticalCheckerIsHit = GetWorld()->LineTraceSingleByChannel(Out2, Start2, End2, ECC_Visibility, CollisionP2);

	if (VerticalCheckerIsHit == true)
	{

		if (Out2.Actor->ActorHasTag("SLIDEDOWN") == true)
		{

			CollisionsForSliding = true;
			GetCapsuleComponent()->SetCapsuleSize(20.0f, 10.0f, true);
			GetMesh()->SetRelativeLocation(FVector(Meshlocation.X-70.0f, Meshlocation.Y, Meshlocation.Z+70.0f), false, 0, ETeleportType::TeleportPhysics);
			if (MoveForwards == true)
			{
				//do once
			}
			if (MoveForwards == false)
			{
				AddMovementInput(Dash->GetForwardVector(), 1.0f, true);
				//do once
			}

		}
		else if (Out2.Actor->ActorHasTag("SLIDEDOWN") == false)
		{

			
			if ()
			{
			}
			
		}









	}
	*/
}
