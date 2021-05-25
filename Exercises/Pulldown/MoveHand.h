// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inputs/Rig/GenericRigPawn.h"
#include "MoveHand.generated.h"


UCLASS()
class SKYFARER_C_API AMoveHand : public AGenericRigPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMoveHand();


	//positive if moving hand left to right
	//FVector(0, 1, -1);
	//negative if moving hand right to left
	//FVector(0,-1,-1);
	UPROPERTY(EditAnywhere, Category = MoveHand)
		FVector angle = FVector(0, 0, 1);
	//whether the hand will move. For switching sides
	UPROPERTY(EditAnywhere, Category = MoveHand)
		bool isActive = true;

	UPROPERTY(EditAnywhere, Category = MoveHand)
		FVector relativeMaxHeight = FVector(0,0,200);
	UPROPERTY(EditAnywhere, Category = MoveHand)
		FVector averageVelocity = FVector(0, 0, 1);

		

	UPROPERTY(EditAnywhere, Category = MoveHand)
		int preferredMaxReps = 20;
	UPROPERTY(EditAnywhere, Category = MoveHand)
		//in seconds
		float speedingPenaltyTime = .5;
	UPROPERTY(EditAnywhere, Category = MoveHand)
		//in seconds
		float  speedPenalty = .01;

	UPROPERTY(EditAnywhere, Category = MoveHand)
		bool usingLeft = true;

	//temp to test out min max position
	UPROPERTY(EditAnywhere, Category = MoveHand)
		float minMouseY = 0; //long minMouseY = 0;
	UPROPERTY(EditAnywhere, Category = MoveHand)
		float maxMouseY = 100;//long maxMouseY = 100;

	UPROPERTY(EditAnywhere, Category = MoveHand)
		int totalNumReps = 4;

	UPROPERTY(VisibleAnywhere, Category = MoveHand)
		float specularness = 0;

	UPROPERTY(VisibleAnywhere, Category = MoveHand)
		float zPos = 0;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	FVector cur_handPosition;
	FVector prev_handPosition;

	FVector2D cur_controllerPosition;
	FVector2D prev_controllerPosition;

	FVector hand_velocity;
	FVector2D controller_velocity;

	//int curHeightLevel = 0;
	//TList<FVector> list_swapPoints;
	
	FVector maxHeight;


	void SetZVelocityRig();
	



	void setXVelocity(float x);
	void setYVelocity(float y);
	void updateHandLocationVector();

	float penaltyTimeLeft = 0;


	UMaterialInstanceDynamic* DynamicMaterial;

	//URigInputManager::SerialData SerialData;
	//URigInputManager* pRigInputManager;

	int maxVelList = 100;
	TArray<FVector> arr_velocities;
	void replaceVelocityInArray(FVector newVelocity);
	float getAverageVelocity();
	void zeroVelocityList();


	void switchHands();

	//LPPOINT curMousePos;
	

	int curRepsDone = 0;

	int totalPoints = 0;
	int curRepPoints = 0;
	int maxPointsPerRep = 1000;

	TArray<FVector> arr_previousAverageVelocities;


	FVector maxVelocity = FVector(0, 0, 2);
	FVector maxVelocity_left = maxVelocity;
	FVector maxVelocity_right = maxVelocity;

	FVector startPullPosition = FVector(0, 0, 1000);
	FVector startPullPosition_left = startPullPosition;
	FVector startPullPosition_right = startPullPosition;

	FVector endPullPosition = FVector(0, 0, 800);
	FVector endPullPosition_left = endPullPosition;
	FVector endPullPosition_right = endPullPosition;

	UStaticMeshComponent* ball;

};
