// Copyright of Night Owls 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cart.generated.h"

class ACarttrack;
class ACabinManager;

UCLASS()
class PAXPROTO_API ACart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACart();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void RunTimeLine(bool Direction);

	UFUNCTION(BlueprintCallable)
	void UpdateTimeLine(float Alpha);

	UFUNCTION(BlueprintCallable)
	void FinishedTimeLine();
	
	UFUNCTION(BlueprintCallable)
	void RightClicked();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Cart")
	UStaticMeshComponent* CartMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Cart")
	ACarttrack* cTrack = nullptr;

	ACabinManager* Manager = nullptr;

	
	bool ForwardDir;

};
