// Copyright of Night Owls 2020 - inclusive © 


#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SplineComponent.h"
#include "Carttrack.h"
#include "CabinManager.h"
#include "Cart.h"



// Sets default values
ACart::ACart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//root guard
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	}

	CartMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cart Mesh"));

}

// Called when the game starts or when spawned
void ACart::BeginPlay()
{
	Super::BeginPlay();

	//Enable player input
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found controller"));
		this->EnableInput(PC);
	}

	//Cast to get track
	cTrack = Cast<ACarttrack>(UGameplayStatics::GetActorOfClass(GetWorld(), ACarttrack::StaticClass()));
	if (cTrack)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found"));
	}

	//Cast to get cabinmanager
	Manager = Cast<ACabinManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACabinManager::StaticClass()));

	//Set up input bindings
	InputComponent->BindAction(FName("RightClick"), EInputEvent::IE_Pressed, this, &ACart::RightClicked);

	//Variable Initialisation
	ForwardDir = true;

}

// Called every frame
void ACart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when right clicked
void ACart::RightClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("CLICKED"));
	//Check we have fully charged up before use
	if (Manager)
	{
		if (Manager->GetCurrentRechargeTime() == Manager->GetRechargeTime())
		{
			RunTimeLine(ForwardDir);
		}
	}
}

//pulse to BP to run timeline
void ACart::RunTimeLine_Implementation(bool Direction)
{

}

//updating from timeline in BP
void ACart::UpdateTimeLine(float Alpha)
{
	if (cTrack)
	{
		//Interpolate to get reference to the spline track position (distance along path)
		float LerpResult = FMath::Lerp(0.0f, cTrack->GetSplineTrack()->GetSplineLength(), Alpha);
		//Use that result to find its new location
		FVector Location = cTrack->GetSplineTrack()->GetLocationAtDistanceAlongSpline(LerpResult, ESplineCoordinateSpace::World);
		//Use that result to find its new rotation, adjusted for forward facing
		FRotator Rotation = UKismetMathLibrary::ComposeRotators(cTrack->GetSplineTrack()->GetRotationAtDistanceAlongSpline(LerpResult, ESplineCoordinateSpace::World), FRotator(0.0f, 270.0f, 0.0f));
		//Set new actor location/rotation
		this->SetActorLocationAndRotation(Location, Rotation);
	}
}
//finishing function from timeline in BP
void ACart::FinishedTimeLine()
{
	if (Manager)
	{
		//Flip Direction
		ForwardDir = !ForwardDir;
		//Reset timer
		Manager->ResetRechargeTime();
	}
}