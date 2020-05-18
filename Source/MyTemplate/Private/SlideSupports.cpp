// Fill out your copyright notice in the Description page of Project Settings.


#include "SlideSupports.h"

// Sets default values
ASlideSupports::ASlideSupports()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");	
	Mesh->SetupAttachment(RootComponent);

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(Mesh);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Mesh);
}

void ASlideSupports::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	
	Character = Cast< ACharacterBase>(OtherActor);
	if (Character != nullptr)
	{
		Character->VerticalCollision = true;
		SlideNow = false;
	}
	

	
}

void ASlideSupports::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bfromSweep, const FHitResult& SweepResult)
{
	
	Character = Cast< ACharacterBase>(OtherActor);
	if (Character != nullptr)
	{
		Character->VerticalCollision = true;
		SlideNow = true;
	}
	
	
	

}

// Called when the game starts or when spawned
void ASlideSupports::BeginPlay()
{
	Super::BeginPlay();
	
	Box->OnComponentBeginOverlap.AddDynamic(this, &ASlideSupports::OnBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ASlideSupports::OnOverlapEnd);
}

// Called every frame
void ASlideSupports::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SlidingTimeline();
}

void ASlideSupports::SlidingTimeline()
{

	if (SlideNow == true)
	{
		if (Character == nullptr)
		{
			return;
		}

		Character->SlideInitiator();
		Character->SlideCollider();
		
		
		//launch velocity
		FVector localSpacedForwardVector = UKismetMathLibrary::InverseTransformDirection(Arrow->GetRelativeTransform(), Arrow->GetForwardVector());
		FVector BounchVel = Character->GetActorForwardVector() + UKismetMathLibrary::InverseTransformDirection(Arrow->GetRelativeTransform(), Arrow->GetForwardVector()) *ArrowForwardVelocity;

		FVector BounchEnhancedVel = BounchVel * BounceVelocity;
		Character->LaunchCharacter(BounchEnhancedVel, false, false);
	}

}

