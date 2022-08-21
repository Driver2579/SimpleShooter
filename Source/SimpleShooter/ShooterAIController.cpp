// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

// Sets default values
AShooterAIController::AShooterAIController() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShooterAIController::BeginPlay() {
	Super::BeginPlay();

	if (AIBehavior) {
		RunBehaviorTree(AIBehavior);

		BlackboardComponent = GetBlackboardComponent();

		if (BlackboardComponent) {
			BlackboardComponent->SetValueAsVector("StartLocation", GetPawn()->GetActorLocation());
			BlackboardComponent->SetValueAsRotator("StartRotation", GetPawn()->GetActorRotation());
		}
	}
}

// Called every frame
void AShooterAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

bool AShooterAIController::IsDead() const {
	AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());

	if (ControlledCharacter) {
		return ControlledCharacter->IsDead();
	}

	return true;
}