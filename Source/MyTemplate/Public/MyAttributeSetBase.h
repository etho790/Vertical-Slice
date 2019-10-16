// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "MyAttributeSetBase.generated.h"

/**
 * AttributeSets are thankfully very simple to explain. They define float values (and ONLY float values. Right now only float
 attributes are supported) and can be connected to AbilitySystems to grant the ability system in question these attributes. 
 GameplayEffects and GameplayEffectExecutionCalculations have specifically designed macros and menus to manipulate these 
 attributes on an ability system. An ability system may use multiple attribute sets or none at all, too.
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangeDelegate, float, Stamina, float, MaxStamina);

UCLASS()
class MYTEMPLATE_API UMyAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()
	

public:
	UMyAttributeSetBase();

	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
		FGameplayAttributeData Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
		FGameplayAttributeData MaxStamina;
	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	FOnHealthChangeDelegate OnStaminaChange;

};
