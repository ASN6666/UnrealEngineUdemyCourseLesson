// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealCPPtestGameMode.h"
#include "UnrealCPPtestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealCPPtestGameMode::AUnrealCPPtestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
