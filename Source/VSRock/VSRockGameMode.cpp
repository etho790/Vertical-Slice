// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "VSRockGameMode.h"
#include "VSRockHUD.h"
#include "VSRockCharacter.h"
#include "UObject/ConstructorHelpers.h"

AVSRockGameMode::AVSRockGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AVSRockHUD::StaticClass();
}
