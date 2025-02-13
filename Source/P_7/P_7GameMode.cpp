// Copyright Epic Games, Inc. All Rights Reserved.

#include "P_7GameMode.h"
#include "P_7Character.h"
#include "UObject/ConstructorHelpers.h"

AP_7GameMode::AP_7GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
