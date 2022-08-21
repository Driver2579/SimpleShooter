// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Shoot.h"
#include "AIController.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
UBTTask_Shoot::UBTTask_Shoot() {
	NodeName = "Shoot";
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller == nullptr) return EBTNodeResult::Failed;
	
	AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(Controller->GetPawn());
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (ControlledCharacter && PlayerPawn) {
		ControlledCharacter->Shoot();

		return EBTNodeResult::Succeeded;
	}
	
	else {
		return EBTNodeResult::Failed;
	}
}