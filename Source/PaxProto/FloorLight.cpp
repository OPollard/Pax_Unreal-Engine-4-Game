// Copyright of Night Owls 2020 - inclusive © 


#include "FloorLight.h"

// Sets default values
AFloorLight::AFloorLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFloorLight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloorLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

