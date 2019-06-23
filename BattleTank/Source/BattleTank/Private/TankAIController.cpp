// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "Tank.h"
#include "Engine/World.h"
#include "TankPlayerController.h"
#include "Components/ActorComponent.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerTank = GetPlayerTank();
	if (!PlayerTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("TankAIController don't find Player tank"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TankAIController found Player tank: %s"), *PlayerTank->GetName());
	}
}

// Called every frame
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetPlayerTank())
	{
		// TODO Move towards the player
		
		// Aim to player
		//GetControlledTank()->AimAt(GetPlayerTank()->GetTargetLocation());

		// Fire if ready
	}
	
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
	ATankPlayerController* TankPlayercontroller = Cast<ATankPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!TankPlayercontroller) { return nullptr; }
	return TankPlayercontroller->GetControlledTank();
}
