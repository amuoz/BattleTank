// Copyright Encumbered Monkey.


#include "Tank.h"
#include "Engine/StaticMeshSocket.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATank::BeginPlay() 
{
	Super::BeginPlay();	// Needed for BP Begin play to run!
}
