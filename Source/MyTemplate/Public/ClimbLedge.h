// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ClimbLedge.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UClimbLedge : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYTEMPLATE_API IClimbLedge
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "LedgeClimb")
	void LedgeClimbingFunction(bool IsClimbing);


};
