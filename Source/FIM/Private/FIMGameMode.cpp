// Copyright Epic Games, Inc. All Rights Reserved.

#include "FIMGameMode.h"
#include "FIMCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFIMGameMode::AFIMGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ProjectFIM/Blueprints/BP_BaseClasses/BP_TestCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
