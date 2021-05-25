// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inputs/Rig/GenericRigPawn.h"
#include "Lever.generated.h"

UCLASS()
class SKYFARER_C_API ALever : public AGenericRigPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALever();

	//values for speed penalty
	UPROPERTY(EditAnywhere, Category = Lever)
		//how long you are penalized for in seconds
		float speedingPenaltyTime = .5;
	UPROPERTY(EditAnywhere, Category = Lever)
		//how much slower you move
		float  speedPenalty = .01;
	UPROPERTY(EditAnywhere, Category = Lever)
		//maximum velocity
		float maxDeltaLocationSize = 3;


	UPROPERTY(EditAnywhere, Category = Lever)
		//how far the lever is allowed to go. Min to max in local soace
		FVector2D localLimits = FVector2D(-50, 50);
	UPROPERTY(VisibleAnywhere, Category = Lever)
		//how far the lever is allowed to go. Min to max in local soace
		FVector2D worldLimits = FVector2D(0, 0);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	//object movement
	void updatePosition(float tick);

	//changing colors
	UMaterialInstanceDynamic* DynamicMaterial;
	float penaltyTimeLeft = 0;
	UStaticMeshComponent* objectMesh;

	int maxVelList = 100;
	TArray<float> arr_velocities;
	void replaceVelocityInArray(float newVelocity);
	float getAverageVelocity();
	void zeroVelocityList();

	//temporary until we connect it to the rig
	void setMouseDeltaLoc(float val);
	//float xVel = 0;
	void checkPenalty();

	void setPenaltyColor(float tick);
	void setOkayColor();



};
