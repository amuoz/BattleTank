// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Classes/Kismet/GameplayStatics.h"

UTankAimingComponent::UTankAimingComponent()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// So that first fire is after initial reload
	LastFireTime = GetWorld()->GetTimeSeconds();
}

void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (FiringState != EFiringState::NoAmmo)
	{
		if ((GetWorld()->GetTimeSeconds() - LastFireTime) < ReloadTimeInSeconds)
		{
			FiringState = EFiringState::Reloading;
		}
		else if (IsBarrelMoving())
		{
			FiringState = EFiringState::Aiming;
		}
		else
		{
			FiringState = EFiringState::Locked;
		}
	}
}

int UTankAimingComponent::GetAmmo() const
{
	return Ammo;
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }
	auto BarrelForward = Barrel->GetForwardVector();
	return !AimDirection.Equals(BarrelForward, 0.02);
}

void UTankAimingComponent::Initialise(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel)) { return; }

	FVector OutLaunchVelocity(0.f);
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	FCollisionResponseParams CollisionResponseParams;
	TArray <AActor*> ActorsToIgnore;

	// Calculate the OutLaunchVelocity
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,	// true for using the high arc solution
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace,
		CollisionResponseParams,
		ActorsToIgnore,
		false	// true activates debug mode auto draws a line
	);

	if (bHaveAimSolution)
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
	}
	// If no solution found do nothing
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel && Turret)) { return; }

	// Work-out difference between current BARREL rotation and AimDirection
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = AimDirection.Rotation();
	FRotator DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Turn(DeltaRotator.GetNormalized().Yaw);
}

void UTankAimingComponent::Fire()
{
	if (FiringState != EFiringState::Reloading && FiringState != EFiringState::NoAmmo)
	{
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBlueprint)) { return; }

		// Spawn a projectile at the socket location on the barrel
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);

		// reset fire time
		LastFireTime = GetWorld()->GetTimeSeconds();

		Ammo--;
		if (Ammo == 0)
		{
			FiringState = EFiringState::NoAmmo;
		}

	}
}
