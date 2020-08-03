// Copyright of Night Owls 2020 - inclusive © 


#include "Money.h"
#include "Kismet/GameplayStatics.h"
#include "PaxState.h"
#include "CabinManager.h"

// Sets default values
AMoney::AMoney()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	
}

// Called when the game starts or when spawned
void AMoney::BeginPlay()
{
	Super::BeginPlay();
	//Get owning pax
	OwningActorComponent = (GetOwner()->FindComponentByClass(UPaxState::StaticClass()));
	//Get that pax's state
	PaxState = Cast<UPaxState>(OwningActorComponent);
	//Get cabin manager
	ResultActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACabinManager::StaticClass());
	//Store CM
	CabinManager = Cast<ACabinManager>(ResultActor);
}

// Called every frame
void AMoney::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//called from blueprint when cursor is overhead
void AMoney::HooveredOver()
{
	if (PaxState)
	{
		//destroy coin actor
		Destroy();
		//Get dropped money and set new money amount on Pax
		const float DroppedMoney = PaxState->GetDeltaSum();
		const float NewMoney = PaxState->GetMoney() - DroppedMoney;
		PaxState->SetMoney(NewMoney);
		//Set this flag to enable new deltasum to be calculated
		PaxState->SetAwaitingPickUp(false);

		if (CabinManager)
		{
			//send dropped money to cabin manager to keep track of
			const float NewTemp = CabinManager->GetTempPickUpAmount() + DroppedMoney;
			CabinManager->SetTempPickUpAmount(NewTemp);
			//enable UI to show collected amount
			CabinManager->RunPickUpUI();
			//increase game earnings
			CabinManager->SetTotalEarnableMoney((CabinManager->GetTotalEarnableMoney()) + DroppedMoney);
		}
		//Reset
		PaxState->ResetDeltaSum();
	}
}

