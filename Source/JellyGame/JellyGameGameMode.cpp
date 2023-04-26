// Copyright Epic Games, Inc. All Rights Reserved.

#include "JellyGameGameMode.h"
#include "JellyGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AJellyGameGameMode::AJellyGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
