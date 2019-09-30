// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "RockHopperGameMode.h"
#include "RockHopperHUD.h"
#include "RockHopperCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARockHopperGameMode::ARockHopperGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ARockHopperHUD::StaticClass();
}
