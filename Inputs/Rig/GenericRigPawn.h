// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CustomDataStructures.h"
#include "Components/StaticMeshComponent.h"
#include "GenericRigPawn.generated.h"

class ARigInputController;
class UGameInstanceManager;

UCLASS()
class SKYFARER_C_API AGenericRigPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGenericRigPawn();

	//ARigInputController::SerialData SerialData;
	ARigInputController* pRigInputManager;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void RollCredits();
	void externalRotation(float value);
	void UsingRigInput();
	void UsingMouseInput();
	void remoteTriggerSpiritAction(int state);
	void remoteTriggerSpiritMovement(int movement);
	static TMap<int32, UCustomDataStructures::CalibrationData> SensorCalibrations;
	UCustomDataStructures::CalibrationData GetCalibration(UCustomDataStructures::RigSensors Sensor);
	FVector CalcDeltaLoc(UCustomDataStructures::RigSensors Sensor);
	float GetPositionDifference(UCustomDataStructures::RigSensors Sensor);
	void IsPlayerInSync(float ExerciseDirection);
	void ResetSyncVars() {
		RepUnsync = false;
		FramesUnsync = 0;
	}
	void TransitionGameState();
	UCustomDataStructures::SerialData UpdateSerialData(UCustomDataStructures::RigSensors Sensor);

	UCustomDataStructures::SerialData SerialData;
	TArray<UCustomDataStructures::RigSensors> ExerciseSensors;
	UGameInstanceManager* UGameManager;
	FVector PreviousRigLocation;
	FVector DeltaLocation;
	int64 TimestampLastDataPull = 0, FramesInDirection = 0;
	FDateTime CurrentTime, TimeOfLastUpdate;
	bool bUsingLeft = false, RepUnsync = false, CheckForExerciseReset = false;
	// Every exercise starts off in the "negative" direction because z sensor values are going down
	float CurrentDirection = -1;
	int FramesUnsync = 0, FramesFrozen = 0, FramesReset = 0, repCount = 0;;

	UFUNCTION(BlueprintCallable, Category = "Exercises")
		void SetTotalReps(int reps) { TotalReps = reps;  }

	UPROPERTY(EditAnywhere)
		bool UsingRig = true;
		
	bool IsCalibration = false;

	TArray<USkeletalMeshComponent*> AttachedMeshes;

	UPROPERTY(EditAnywhere)
		int TotalReps = 3;
		int FramesThresholdToDetermineDirection = 3;
};
