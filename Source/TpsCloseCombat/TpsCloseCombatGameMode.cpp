// Copyright Epic Games, Inc. All Rights Reserved.

#include "TpsCloseCombatGameMode.h"
#include "TpsCloseCombatCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATpsCloseCombatGameMode::ATpsCloseCombatGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
