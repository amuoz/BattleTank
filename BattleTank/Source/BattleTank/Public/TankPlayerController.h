// Copyright Encumbered Monkey.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

// Forward declaration
class UTankAimingComponent;

/**
 * Tank player controller
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
		
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* AimCompRef);

private:

	// CrossHairX is located 50% on the screen
	UPROPERTY(EditDefaultsOnly)
	float CrossHairXLocation = 0.5f;
	
	// CrossHairY is located 33% on the screen
	UPROPERTY(EditDefaultsOnly)
	float CrossHairYLocation = 0.33333f;

	// max ray-cast range
	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000.f;	// cm

	// Start the tank moving the barrel so that a shot would hit
	// where the crosshair intersects the world
	void AimTowardsCrosshair();

	// Return an OUT parameter, true if hit landscape
	bool GetSightRayHitLocation(FVector& OutHitLocation) const;

	// De-project the screen position of the crosshair to a world direction
	bool GetLookDirection(FVector& LookDirection) const;

	bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;

	virtual void SetPawn(APawn* InPawn) override;

	UFUNCTION()
	void OnPossedTankDeath();
};
