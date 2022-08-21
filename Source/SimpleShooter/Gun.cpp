// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGun::AGun() {
	Root = CreateDefaultSubobject<USceneComponent>("Root Component");
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);

	MaxRange = 10000;
	Damage = 5;
}

// Called when the game starts or when spawned
void AGun::BeginPlay() {
	Super::BeginPlay();
}

bool AGun::SetupGunTrace(FHitResult& HitResult, FVector& ShotDirection) {
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return false;

	FVector Location;
	FRotator Rotation;

	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxRange;

	FCollisionQueryParams Params;

	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Location,
		End,
		ECC_GameTraceChannel1,
		Params
	);
}

AController* AGun::GetOwnerController() const {
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;

	return OwnerPawn->GetController();
}

void AGun::PullTrigger() {
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, "MuzzleFlashSocket");
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, "MuzzleFlashSocket");

	FHitResult HitResult;
	FVector ShotDirection;

	bool bSucces = SetupGunTrace(HitResult, ShotDirection);

	if (bSucces) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ImpactEffect,
			HitResult.Location,
			ShotDirection.Rotation()
		);

		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ImpactSound,
			HitResult.Location
		);
		
		AActor* HitActor = HitResult.GetActor();

		if (HitActor) {
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			HitActor->TakeDamage(Damage, DamageEvent, GetOwnerController(), this);
		}
	}
}