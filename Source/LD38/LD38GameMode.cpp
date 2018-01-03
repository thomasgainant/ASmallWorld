// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "LD38.h"
#include "LD38GameMode.h"
#include "LD38Character.h"

ALD38GameMode::ALD38GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
