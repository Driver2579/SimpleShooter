// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AShooterPlayerController::AShooterPlayerController() {
	RestartDelay = 5;
}

// Called when the game starts or when spawned
void AShooterPlayerController::BeginPlay() {
	Super::BeginPlay();

	HUD = CreateWidget(this, HUDClass);

	if (HUD) {
		HUD->AddToViewport();
	}
}

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner) {
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (bIsWinner) {
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);

		if (WinScreenClass) {
			WinScreen->AddToViewport();
		}
	}

	else {
		UUserWidget* LooseScreen = CreateWidget(this, LoseScreenClass);

		if (LooseScreen) {
			LooseScreen->AddToViewport();
		}
	}

	if (HUD) {
		HUD->RemoveFromViewport();
	}

	GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &APlayerController::RestartLevel, RestartDelay);
}