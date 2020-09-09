// Copyright of Night Owls 2020 - inclusive © 


#include "Carttrack.h"
#include "Components/SplineComponent.h"

// Sets default values
ACarttrack::ACarttrack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Root guard
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	}
	//Create cart track component
	CartTrack = CreateDefaultSubobject<USplineComponent>(TEXT("Track"));
	CartTrack->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void ACarttrack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACarttrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Getter reference
USplineComponent* ACarttrack::GetSplineTrack()
{
	return CartTrack;
}
