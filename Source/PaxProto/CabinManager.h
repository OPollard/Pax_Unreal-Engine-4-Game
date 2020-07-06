// Copyright of Night Owls 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CabinManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FManagerDelegate);

class UPaxState;

UCLASS()
class PAXPROTO_API ACabinManager : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(BlueprintAssignable)
	FManagerDelegate TakeOffPulse;

	ACabinManager();

	virtual void Tick(float DeltaTime) override;

	float GetTotalEarnableMoney();
	UFUNCTION(BlueprintPure)
	FText GetTotalEarnableMoneyAsText();

	void SetTotalEarnableMoney();
	void SetTotalEarnableMoney(float x);

	void SetTempPickUpAmount(float x);
	UFUNCTION(BlueprintPure)
	float GetTempPickUpAmount();
	UFUNCTION(BlueprintPure)
	FText GetTempPickUpAmountAsText();

	void RunPickUpUI();
	void ResetPickUpUI();

	UFUNCTION(BlueprintPure)
	bool GetPickUpUIStatus();
	
	UFUNCTION()
	void RegisterNewPax(AActor* NewActor);

	UFUNCTION(BlueprintPure)
	int32 GetNumPaxOnboard();

protected:
	
	virtual void BeginPlay() override;

private:	

	void Initialise();

	UFUNCTION()
	void PaxHasDied();

	UFUNCTION()
	void CheckDoorStatus();
	void Launch();

	

	FTimerHandle ManagerHandle;
	
	TArray<AActor*> Passengers;
	TArray<UPaxState*> PassengerStates;
	TArray<AActor*> Doors;

	int32 PaxOnboard{ 0 };

	float TotalEarnableMoney{ 0.0f };
	float TempPickUpAmount{ 0.0f };

	bool EnablePickUpUI{ false };

};
