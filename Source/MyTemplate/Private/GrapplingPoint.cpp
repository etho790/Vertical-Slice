#include "GrapplingPoint.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AGrapplingPoint::AGrapplingPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Point = CreateDefaultSubobject<UStaticMeshComponent>("Point");
	Point->SetCollisionProfileName(TEXT("NoCollision"));


	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetupAttachment(Point);
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

}

void AGrapplingPoint::ChangeToBaseMat()
{		//!!!!!!!!!!!!!MUST PUT THE COLOUR CHANGE IN THE CHARACTER CLASS
	if (BaseMaterial)
	{
		Point->SetMaterial(0, BaseMaterial);
	}
}

void AGrapplingPoint::ChangeToGrapplingMat()
{
	//!!!!!!!!!!!!!MUST PUT THE COLOUR CHANGE IN THE CHARACTER CLASS
	if (GrapplingMaterial)
	{
		Point->SetMaterial(0, GrapplingMaterial);
	}
}

void AGrapplingPoint::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bfromSweep, const FHitResult& SweepResult)
{
	Character = Cast< ACharacterBase>(OtherActor);

	if (Character != nullptr)
	{
		Character->GrappleComponent->DetachFromGrappling();
		Character->GrappleComponent->GrappleTimer = 0;
	}
}

void AGrapplingPoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	Character = Cast< ACharacterBase>(OtherActor);
	if (Character != nullptr)
	{
		Character->GrappleComponent->DetachFromGrappling();
		
		
		//Character->GrappleComponent->GrappleTimer = 0;
	}


	Character = nullptr;
	
}
