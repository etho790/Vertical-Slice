// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"

#include "CharacterBase.generated.h"

class UMyAttributeSetBase;

UCLASS()
class MYTEMPLATE_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override //We add this function, overriding it from IAbilitySystemInterface.
	{
		return AbilitySystemComp;
	};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
		class UAbilitySystemComponent* AbilitySystemComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
		UMyAttributeSetBase* AttributeSetBaseComp;


	//  Give the character an ability of choice. 
	//  For simplicity's sake we will only give him one on the action mapping "UseAbility1" 
	//	and just give the actor a variable that defines which ability to put there, but the same principles for
	//	granting one ability are applicable for multiple ones. We will make it blueprint-editable too so we can 
	//	easily change the ability we want to test later down the line.
	//	Our variable will be a TSubclassOf<UGameplayAbility>, because we get all relevant info from the class alone.In fact,
	//	GameplayAbilities can be set up to only instance per activation or not to instance at all even, so giving an 
	//	instance we can freely change beforehand would be a weird idea, anyway.

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
		void AcquireAbility (TSubclassOf<class UGameplayAbility> Ability);

	UFUNCTION()
		void OnStaminaChanged(float Stamina, float MaxStamina);
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnStaminaChanged"))
		void BP_OnStaminaChanged(float Stamina, float MaxStamina);


};
