// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Inputs/Rig/GenericRigPawn.h"
#include "PaddleRotation.generated.h"

UCLASS()
class SKYFARER_C_API APaddleRotation : public AGenericRigPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APaddleRotation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Paddling(float value);

	UCustomDataStructures::SerialData SerialData;

	bool ReachedPaddleLimit = false;
	float inc = -1.0f;

	UStaticMeshComponent* Paddle;

	FVector Waypoints[3] = {
		{ 2170.000000, 4270.000000, 80.000000 },
		{ 1480.000000, 500.000000, 80.000000 },
		{ -560.000000,-3850.000000, 80.000000 }
	};
};
