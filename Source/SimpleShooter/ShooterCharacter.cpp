// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacter.h"

#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Components/CapsuleComponent.h"

#include "Gun.h"

#include "Kismet/GameplayStatics.h"

#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);

	SpringArmComponent->bUsePawnControlRotation = true;
	
	//bUseControllerRotationYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	MinPitchView = -25;
	MaxPitchView = 35;

	/*bIsMovingForward = false;
	bIsMovingRight = false;*/

	RotationRate = 75;

	MaxHealth = 100;

	bCanShoot = true;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay() {
	Super::BeginPlay();
	
	GetMesh()->HideBoneByName("weapon_r", EPhysBodyOp::PBO_None);

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "WeaponSocket");
	Gun->SetOwner(this);

	Health = MaxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis("LookRightRate", this, &AShooterCharacter::LookRightRate);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AShooterCharacter::Shoot);
}

void AShooterCharacter::MoveForward(float Axis) {
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0, Rotation.Yaw, 0);

	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(Direction, Axis);

	/*if (Axis != 0 && !bIsMovingForward) {
		bIsMovingForward = true;
		SwitchCharacterRotation();
	}

	else if (Axis == 0 && bIsMovingForward) {
		bIsMovingForward = false;
		SwitchCharacterRotation();
	}*/
}

void AShooterCharacter::MoveRight(float Axis) {
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0, Rotation.Yaw, 0);

	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Direction, Axis);

	/*if (Axis != 0 && !bIsMovingRight) {
		bIsMovingRight = true;
		SwitchCharacterRotation();
	}

	else if (Axis == 0 && bIsMovingRight) {
		bIsMovingRight = false;
		SwitchCharacterRotation();
	}*/
}

void AShooterCharacter::LookUpRate(float Axis) {
	float Speed = Axis * RotationRate * GetWorld()->GetDeltaSeconds();
	AddControllerPitchInput(Speed);
}

void AShooterCharacter::LookRightRate(float Axis) {
	float Speed = Axis * RotationRate * GetWorld()->GetDeltaSeconds();
	AddControllerYawInput(Speed);
}

//void AShooterCharacter::SwitchCharacterRotation() {
//	if (!bUseControllerRotationYaw && !GetCharacterMovement()->bOrientRotationToMovement) {
//		UE_LOG(LogTemp, Display, TEXT("Switch to true"));
//		
//		bUseControllerRotationYaw = true;
//		GetCharacterMovement()->bOrientRotationToMovement = true;
//	}
//
//	else if (!bIsMovingForward && !bIsMovingRight) {
//		UE_LOG(LogTemp, Display, TEXT("Switch to false"));
//
//		bUseControllerRotationYaw = false;
//		GetCharacterMovement()->bOrientRotationToMovement = false;
//	}
//}

void AShooterCharacter::Shoot() {
	if (bCanShoot) {
		Gun->PullTrigger();
	}
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// Min() returns lower value in a generic way
	DamageToApply = FMath::Min(Health, DamageToApply);

	Health -= DamageToApply;
	
	if (IsDead()) {
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
	
		if (GameMode != nullptr) {
			GameMode->PawnKilled(this);
		}

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	UE_LOG(LogTemp, Display, TEXT("Health: %f"), Health);
	return DamageToApply;
}

bool AShooterCharacter::IsDead() const {
	if (Health <= 0) {
		return true;
	}

	return false;
}

float AShooterCharacter::GetHealth() const {
	return Health;
}

float AShooterCharacter::GetHealthPercent() const {
	// Formule: (n * 100 / N) / 100 = n / N = P
	return Health / MaxHealth;
}