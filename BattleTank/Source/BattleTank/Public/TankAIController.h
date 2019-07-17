// Copyright Encumbered Monkeys.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class ATank;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// How close can the AI tank get
	UPROPERTY(EditAnywhere, Category = "Setup")	// Consider EditDefaultsOnly
	float AcceptanceRadius = 8000;

private:

	virtual void BeginPlay() override;

	virtual void SetPawn(APawn* InPawn) override;

	UFUNCTION()
	void OnPossedTankDeath();
};
