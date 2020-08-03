// Copyright of Night Owls 2020 - inclusive © 

#include "CabinManager.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "AIController.h"
#include "Kismet/KismetTextLibrary.h"
#include "Constants.h"
#include "Pax.h"
#include "PaxState.h"
#include "Door.h"
#include "Seat.h"

// Sets default values
ACabinManager::ACabinManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACabinManager::BeginPlay()
{
	Super::BeginPlay();

	//Redirects equivalent begin play function to enable other actors to spawn first
	GetWorldTimerManager().SetTimer(ManagerHandle, this, &ACabinManager::Initialise, 1.0f, false, 0.0f);
}

// Called every frame
void ACabinManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Initialise function, delayed to ensure other actors are spawned
void ACabinManager::Initialise()
{
	//Get Door Actors
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoor::StaticClass(), Doors);
	for (auto& D : Doors)
	{
		if (ADoor* DPtr = Cast<ADoor>(D))
		{
			DPtr->DoorMoved.BindDynamic(this, &ACabinManager::CheckDoorStatus); //single delegate binding from door
		}
	}

	//Get Seat Actors
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASeat::StaticClass(), Seats);
	NumSeatsInLevel = Seats.Num();

	//Game play Timer Tracker
	GetWorldTimerManager().SetTimer(LevelTimeHandle, this, &ACabinManager::TimerTick, 1.0f, true, 1.0f);

	//Variables
	FlightPhase = EPhase::BOARDING;
	LevelBoardingTime = FREEFLIGHT_BOARDINGTIME;
	LevelFlightTime = FREEFLIGHT_CRUISETIME;
	//Fetch so the GAME UI works
	SetTotalEarnableMoney(0);
}

//Fetch Remaining SegmentTime
float ACabinManager::GetRemainingTime()const
{
	if (FlightPhase == EPhase::BOARDING)
		return LevelBoardingTime;
	else if (FlightPhase == EPhase::CRUISE)
		return LevelFlightTime;
	else return 1901.0f;
}



//Return total possible money earned on level
float ACabinManager::GetTotalEarnableMoney()const
{
	return TotalEarnableMoney;
}

// Returns money stat as FText format for the UI
FText ACabinManager::GetTotalEarnableMoneyAsText()const
{
	FString T_Money{ FString::SanitizeFloat(TotalEarnableMoney, 0) };
	FString T_Display{ "$" };
	T_Display.Append(T_Money);
	return UKismetTextLibrary::Conv_StringToText(T_Display);
}

//Set Total earnable money based on all pax personal money combined
void ACabinManager::SetTotalEarnableMoney()
{
	for (const auto& p : PassengerStates)
	{
		TotalEarnableMoney += p->GetMoney();
	}
}

//Set total earnable money to specific parameter amount
void ACabinManager::SetTotalEarnableMoney(const float X)
{
	TotalEarnableMoney = X;
}

void ACabinManager::SetTempPickUpAmount(const float X)
{
	TempPickUpAmount = X;
}

float ACabinManager::GetTempPickUpAmount()const
{
	return TempPickUpAmount;
}

FText ACabinManager::GetTempPickUpAmountAsText()const
{
	FString Pu_Money{ FString::SanitizeFloat(TempPickUpAmount, 0) };
	FString Pu_Display{};
	(TempPickUpAmount >= 0) ? Pu_Display.Append("+") : Pu_Display.Append("-");
	Pu_Display.Append("$");
	Pu_Display.Append(Pu_Money);
	return UKismetTextLibrary::Conv_StringToText(Pu_Display);
}


//set pickup UI for timer amount of 2.25 seconds
void ACabinManager::RunPickUpUI()
{
	EnablePickUpUI = true;
	GetWorldTimerManager().SetTimer(ManagerHandle, this, &ACabinManager::ResetPickUpUI, 1.0f, false, 2.25f);
}

void ACabinManager::ResetPickUpUI()
{
	EnablePickUpUI = false;
	TempPickUpAmount = 0;
}

bool ACabinManager::GetPickUpUIStatus()const
{
	return EnablePickUpUI;
}




//Called when event dispatched from pax state
void ACabinManager::PaxHasDied()
{
	//cycle through pax states on record
	for (const auto& PState : PassengerStates)
	{
		if (!PState->IsAlive()) //Check any particular pax is Alive
		{
			if (APax* P = Cast<APax>(PState->GetOwner())) //find its AActor
			{
				if (AController* PaxAICon = P->GetController()) //Get Its Controller (AI in this case)
				{
					PaxAICon->UnPossess();	//Remove any activation of that PAX
				}
			}
		}
	}
}


//Register the new pax on manifest
void ACabinManager::RegisterNewPax(AActor* NewActor)
{
	if (UPaxState* NewPax = Cast<APax>(NewActor)->FindComponentByClass<UPaxState>())
	{
		//Add to array on manifest
		PassengerStates.Add(NewPax);
		++PaxOnboard; //add to the number of pax onboard, array.num() not working

		//Register to await for death
		NewPax->OnDeath.AddUniqueDynamic(this, &ACabinManager::PaxHasDied);
	
		NewPax->SetOnboard(true);
	}
}

//Get number of pax onboard
int32 ACabinManager::GetNumPaxOnBoard()const
{
	return PaxOnboard;
}

//Get number of seats in level
int32 ACabinManager::GetNumSeatsInLevel()const
{
	return NumSeatsInLevel;
}

//Get remaining seats
int32 ACabinManager::GetRemainingSeats()const
{
	return NumSeatsInLevel - PaxOnboard;
}

//Get percent of airplane seats occupied
float ACabinManager::GetLoadFactor() const
{

	if (NumSeatsInLevel)
	{
		const float LocalPax = PaxOnboard;
		const float LocalSeats = NumSeatsInLevel;
		const float LocalRatio = (LocalPax / LocalSeats) * 100.0f;
		
		return LocalRatio;
	}

	return 0.0f;
}

//set a new current flight phase
void ACabinManager::SetCurrentPhase(const EPhase NewPhase)
{
	FlightPhase = NewPhase;
}

//return the current flight phase eg cruise
EPhase ACabinManager::GetCurrentPhase()const
{
	return FlightPhase;
}


// called once from door event delegate
void ACabinManager::CheckDoorStatus()
{
	//keep track of number of doors
	size_t Counter{ 0 };
	for (const auto& d : Doors)
	{
		if (ADoor* DPtr = Cast<ADoor>(d))
		{
			if (!DPtr->GetDoorStatus()) { ++Counter; } // is this door closed then add to number open
		}
	}
	if (Counter == Doors.Num()) //we want all closed
	{
		Launch(); //send to launch protocol
	}
}

//Decrease Level Timer
void ACabinManager::TimerTick()
{
	//if in boarding phase, tick boarding timer down
	//if timer = 0, launch
	if (FlightPhase == EPhase::BOARDING)
	{
		//If any time is left, decrease when called
		if (LevelBoardingTime) { LevelBoardingTime--; }
		else
		{
			//then check if we haven't launched already
			if (GoForLaunch)
			{
				Launch(); //send to launch protocol
				//GetWorldTimerManager().ClearTimer(LevelTimeHandle); //clear any timer on this
			}
		}
	}
	//if in cruising phase, tick cruising timer down
	//if timer = 0, land
	else if (FlightPhase == EPhase::CRUISE)
	{
		if (LevelFlightTime) { LevelFlightTime--; }
		else
		{
			GetWorldTimerManager().ClearTimer(LevelTimeHandle);//clear timer
			UE_LOG(LogTemp, Warning, TEXT("LANDED"));
		}
	}

	
}

//Pulse Launch Protocol
void ACabinManager::Launch()
{
	//Check not been launched already
	if(GoForLaunch)	TakeOffPulse.Broadcast();
	GoForLaunch = false; //prevent more launches
	UE_LOG(LogTemp, Warning, TEXT("LAUNCH")); //temp for debugging

	//Search all actors, prior to checking if OnBoard
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APax::StaticClass(), Passengers);
	for (auto p : Passengers)
	{
		if (UPaxState* Ps = Cast<APax>(p)->FindComponentByClass<UPaxState>())
		{
			//if not OnBoard, get owning actor, and destroy
			if (!Ps->GetOnboard()) Ps->GetOwner()->Destroy();
		}
	}


}
