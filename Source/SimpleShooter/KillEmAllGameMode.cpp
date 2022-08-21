// Fill out your copyright notice in the Description page of Project Settings.

#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* Pawn) {
	Super::PawnKilled(Pawn);

	APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());

	if (PlayerController) {
		EndGame(false);
	}

	else {
		for (AShooterAIController* AIController : TActorRange<AShooterAIController>(GetWorld())) {
			if (!AIController->IsDead()) {
				return;
			}
		}

		EndGame(true);
	}
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner) {
	for (AController* Controller : TActorRange<AController>(GetWorld())) {
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}