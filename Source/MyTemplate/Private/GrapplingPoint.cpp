#include "GrapplingPoint.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AGrapplingPoint::AGrapplingPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");

	
	Point = CreateDefaultSubobject<UStaticMeshComponent>("Point");
	Point->SetupAttachment(Scene);


	
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetupAttachment(Point);

	//ADDED ENOCH!!!!!!!!!
	TimerBeforeColorChanges = 0;
	
}

// Called when the game starts or when spawned
void AGrapplingPoint::BeginPlay()
{
	Super::BeginPlay();

	Collider->OnComponentBeginOverlap.AddDynamic(this, &AGrapplingPoint::OnBeginOverlap);
	Collider->OnComponentEndOverlap.AddDynamic(this, &AGrapplingPoint::OnOverlapEnd);

		
}

// Called every frame
void AGrapplingPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	ChangeToBaseMat(DeltaTime);
	
}

void AGrapplingPoint::ChangeToBaseMat(float DeltaTime)
{	
	
	if (BaseMaterial)
	{
		if (ColorChanged == true)
		{
			TimerBeforeColorChanges += DeltaTime;
			if (TimerBeforeColorChanges > 0.05f)
			{
				Point->SetMaterial(0, BaseMaterial);
				ColorChanged = false;
			}
		}
	}
}

void AGrapplingPoint::ChangeToGrapplingMat()
{
	
	if (GrapplingMaterial)
	{
		Point->SetMaterial(0, GrapplingMaterial);
		ColorChanged = true;
		TimerBeforeColorChanges = 0;
	}
}

void AGrapplingPoint::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bfromSweep, const FHitResult& SweepResult)
{
	Character = Cast< ACharacterBase>(OtherActor);

	if (Character != nullptr)
	{
	
		
			Character->GrappleComponent->DetachFromGrapplingOnceHit();
			Character->GrappleComponent->GrappleTimer = 0;
			
		
	}
}

void AGrapplingPoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	Character = Cast< ACharacterBase>(OtherActor);
	if (Character != nullptr)
	{
		
	}


	Character = nullptr;
	
}
