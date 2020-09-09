// Copyright of Night Owls 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Carttrack.generated.h"

class USplineComponent;

UCLASS()
class PAXPROTO_API ACarttrack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACarttrack();

	UPROPERTY(EditAnywhere, Category = "Track")
	USplineComponent* CartTrack = nullptr;

	USplineComponent* GetSplineTrack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
