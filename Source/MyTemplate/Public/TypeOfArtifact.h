// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TypeOfArtifact.generated.h"
/**
 * 
 */

UENUM(BlueprintType)
enum class PickupType : uint8
{
	Homing_Missile  UMETA(DisplayName = "Missile"),
	Flash_Bomb		UMETA(DisplayName = "Bomb"),
	ThirdPickup		UMETA(DisplayName = "LastPickup")
};
