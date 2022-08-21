// Copyright Epic Games, Inc. All Rights Reserved.


#include "SimpleShooterGameModeBase.h"

void ASimpleShooterGameModeBase::PawnKilled(APawn* Pawn) {
	UE_LOG(LogTemp, Display, TEXT("Oops! Someone died!"));
}