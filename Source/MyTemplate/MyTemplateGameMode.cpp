// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MyTemplateGameMode.h"
#include "MyTemplateHUD.h"
#include "MyTemplateCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMyTemplateGameMode::AMyTemplateGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AMyTemplateHUD::StaticClass();
}
