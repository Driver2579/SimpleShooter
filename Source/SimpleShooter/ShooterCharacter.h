// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* Camera;

	void MoveForward(float Axis);
	void MoveRight(float Axis);

	void LookUpRate(float Axis);
	void LookRightRate(float Axis);

	/*void SwitchCharacterRotation();
	
	bool bIsMovingForward;
	bool bIsMovingRight;*/

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotationRate;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AGun> GunClass;

	UPROPERTY()
	AGun* Gun;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth;

	UPROPERTY(VisibleAnywhere)
	float Health;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealth() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	void Shoot();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MinPitchView;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MaxPitchView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanShoot;
};