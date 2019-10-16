// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "MyAttributeSetBase.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//make the variable you see in blueprint editor ******************
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSetBaseComp = CreateDefaultSubobject<UMyAttributeSetBase>(TEXT("AttributeSetBaseComponent"));
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{

	Super::BeginPlay();
	AttributeSetBaseComp->OnStaminaChange.AddDynamic(this, &ACharacterBase::OnStaminaChanged);
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBase::AcquireAbility(TSubclassOf<class UGameplayAbility> Ability)
{

	// Initializes ability
	if (AbilitySystemComp)
	{
		if (HasAuthority() && Ability)
		{
			AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, 0));
		}
		AbilitySystemComp->InitAbilityActorInfo(this, this);
	}

}

void ACharacterBase::OnStaminaChanged(float Stamina, float MaxStamina)
{
	BP_OnStaminaChanged(Stamina, MaxStamina);
}



