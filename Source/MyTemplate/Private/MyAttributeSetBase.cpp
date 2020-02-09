// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAttributeSetBase.h"

#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"

UMyAttributeSetBase::UMyAttributeSetBase() : Stamina(75.0f), MaxStamina(100.0f)
{

}


// happens after this evaluation and as such you are unable to throw the GameplayEffectExecution 
// out properly by then. However, because 90% of the time things such as damage calculations will be effect executions,
// here will be an excellent place to wrap such a thing up, such as by, for example, checking if the damage you took killed you.

void UMyAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data)
{

	// checking if attribute changed is stamina then do something
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UMyAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UMyAttributeSetBase, Stamina)))
	{
		Stamina.SetCurrentValue(FMath::Clamp(Stamina.GetCurrentValue(), 0.0f, MaxStamina.GetCurrentValue()));
		Stamina.SetBaseValue(FMath::Clamp(Stamina.GetBaseValue(), 0.0f, MaxStamina.GetCurrentValue()));
		OnStaminaChange.Broadcast(Stamina.GetCurrentValue(), MaxStamina.GetCurrentValue());
	}
	
}