// Copyright of Night Owls 2020 - inclusive © 

#include "Gate.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/SpotLightComponent.h"
#include "CabinManager.h"
#include "Pax.h"

// Sets default values
AGate::AGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	}

	GateBox = CreateDefaultSubobject<UBoxComponent>(TEXT("GateBox"));
	GateBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	GateBox->SetBoxExtent(FVector(45.0f, 45.0f, 60.0f));
	GateBox->SetNotifyRigidBodyCollision(true);

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spot Light"));
	SpotLight->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpotLight->SetRelativeLocation(FVector(0.0f, 0.0f, 104.0f));
	SpotLight->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	SpotLight->SetIntensity(2000.0f);
	SpotLight->SetOuterConeAngle(30.0f);
	SpotLight->SetCastShadows(false);

}

// Called when the game starts or when spawned
void AGate::BeginPlay()
{
	Super::BeginPlay();

	//get manager handle
	Manager = Cast<ACabinManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACabinManager::StaticClass()));
	//Destroy self on takeoff initiation
	Manager->TakeOffPulse.AddUniqueDynamic(this, &AGate::TakeOffRecieve);
	//Bind overlap to check for spawn
	GateBox->OnComponentEndOverlap.AddDynamic(this, &AGate::CustomOnComponentEndOverlap);

	//Resets Static Variable
	PaxSelector = 0;
	
}

// Called every frame
void AGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//custom end overlap function, to spawn pax if clear
void AGate::CustomOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//If passengers are left
	if (Manager->Passengers.Num() > 0)
	{
		//and left mouse button is released, eg not holding a pax
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!PC->IsInputKeyDown(EKeys::LeftMouseButton))
		{
			//Allow first pax to move out, before spawning
			GetWorldTimerManager().SetTimer(PaxSpawnInterval, this, &AGate::MovePax, 0.3f, false);
		}
	}
}

// Move passenger from its location in pax spawner (handle in manager) to the gate location
void AGate::MovePax()
{
	//Clear Timer First
	GetWorldTimerManager().ClearTimer(PaxSpawnInterval);

	if (EnableSpawn)
	{
		if (Manager)
		{
			//points to next pax, static PaxSelector, consistent across instances
			APax* P = Manager->Passengers[PaxSelector];
			if (P)
			{
				//move pax to the gate area from spawn area
				P->SetActorLocation(this->GetActorLocation());
				//if pax is not pointing at last element
				if (!(PaxSelector == (Manager->Passengers.Num() - 1)))
				{
					//then increase it
					PaxSelector++;
				}
				else
				{
					//otherwise disable spawn
					EnableSpawn = false;
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No pax found to be selected"));
			}
		}
	}
}

//destroy self
void AGate::TakeOffRecieve()
{
	EnableSpawn = false;
	this->Destroy();
}