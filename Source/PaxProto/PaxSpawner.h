// Copyright of Night Owls 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaxSpawner.generated.h"

class UBoxComponent;
class APax;

UCLASS()
class PAXPROTO_API APaxSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APaxSpawner();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* SpawnArea = nullptr;

	void GetSpawnPoint(const int32 NumOfPaxToSpawn, FVector& Location);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool Q; //represents turn phase
	FVector SpawnPoint;

	bool CheckWithinBounds(const FVector& Point);
};
