// Fill out your copyright notice in the Description page of Project Settings.


#include "MUI_JumpPad.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "CharacterBase.h"

// Sets default values
AMUI_JumpPad::AMUI_JumpPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereRadius = 100.0f;
	FullJumpPower = 3.0f;

	MyCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("My Sphere Comp"));
	MyCollisionSphere->InitSphereRadius(SphereRadius);
	MyCollisionSphere->SetCollisionProfileName("Trigger");

	RootComponent = MyCollisionSphere;
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh Comp"));
	MyMesh->SetupAttachment(RootComponent);

	MyCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AMUI_JumpPad::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AMUI_JumpPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMUI_JumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMUI_JumpPad::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacterBase* jumpPlayer = Cast<ACharacterBase>(OtherActor);
	if (jumpPlayer != nullptr)
	{
		float jumpZvalue = FullJumpPower * jumpPlayer->GetCharacterMovement()->JumpZVelocity;
		jumpPlayer->LaunchCharacter(FVector(0.0f, 0.0f, jumpZvalue), false, true);
	}
}

