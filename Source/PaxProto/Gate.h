// Copyright of Night Owls 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gate.generated.h"

class ACabinManager;
class UBoxComponent;
class USpotLightComponent;

UCLASS()
class PAXPROTO_API AGate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGate();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void MovePax();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* GateBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpotLightComponent* SpotLight = nullptr;

	UFUNCTION()
	void CustomOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void TakeOffRecieve();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	FTimerHandle PaxSpawnInterval;

	ACabinManager* Manager = nullptr;
	bool EnableSpawn{ true };

	static int32 PaxSelector;

}; int32 AGate::PaxSelector = 0;
