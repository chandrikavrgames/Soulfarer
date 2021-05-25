// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inputs/Rig/GenericRigPawn.h"
#include "Components/SplineComponent.h"
#include "Spirit_temp.generated.h"

UCLASS()
class SKYFARER_C_API ASpirit_temp : public AGenericRigPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpirit_temp();

	UPROPERTY(EditAnywhere, Category = Spirit_temp)
		//whether it is activated
		bool activated = false;

	UPROPERTY(EditAnywhere, Category = Spirit_temp)
		//how fast it spins
		FVector spinSpeed = FVector(2,4,6);

	UPROPERTY(EditAnywhere, Category = Spirit_temp)
		//how fast it spins
		FVector activatedSpinSpeed = FVector(10, 20, 30);

	UPROPERTY(EditAnywhere, Category = Spirit_temp)
		//color
		FLinearColor spiritColor = FLinearColor(.25, .5, 1);

	UPROPERTY(EditAnywhere, Category = Spirit_temp)
		float RiseSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category = Spirit_temp)
		float RiseDistance = 350.0f;

	UPROPERTY(EditAnywhere, Category = Spirit_temp)
		float SinkDistance = -100.0f;

	void TriggerSpiritAction(int state);

	int spiritState = 3;
	int actionState = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void spiritMovement(int movement);

	UMaterialInstanceDynamic* DynamicMaterial;
	USkeletalMeshComponent* objectMesh;

	FVector StartPosition;
	USplineComponent* SplineComponent;
	FVector pathPointLocation[500000];//save sampled point locations into an array
	FQuat pathPointRotation[500000];//save sampled point rotations into an array
	int totalSplinePoints = 0; //After we sampled the spline at intervals, this is the total number of sampled points on the curve
	int splinePointer = 0;
	
};
