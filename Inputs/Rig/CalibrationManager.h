// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "RenderCore.h"
#include "CustomDataStructures.h"
#include "CoreMinimal.h"
#include "Inputs/Rig/GenericRigPawn.h"
#include "CalibrationManager.generated.h"

UCLASS()
class SKYFARER_C_API ACalibrationManager : public AGenericRigPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACalibrationManager();
	~ACalibrationManager();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Calibrate(float DeltaTime);
	float CheckForVelocityReversal(float Direction);
	bool CheckForMovement(bool isRelaxing, float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Calibration")
	float GetCalibrationProgress();

	UFUNCTION(BlueprintCallable, Category = "Calibration")
	int GetSensorNum();

	UFUNCTION(BlueprintCallable, Category = "Calibration")
	int DecrementCalibrationSensor();

	UFUNCTION(BlueprintCallable, Category = "Calibration")
	ECalibrationCounter BeginCalibration();

	UFUNCTION(BlueprintCallable, Category = "Calibration")
	bool FinishedCurrentCalibration();

	UFUNCTION(BlueprintCallable, Category = "Calibration")
	FString peanutbutter();

	UFUNCTION(BlueprintCallable, Category = "Calibration")
	bool SetName(FString Name);

	UFUNCTION(BlueprintCallable, Category = "Calibration")
	FString ShowUser(FString FullName);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Calibration")
	void ReadyToPlay();

	UFUNCTION(BlueprintCallable, Category = "Calibration")
	TArray<FString> GetAllPlayerNames();

	bool CheckIfPlayerExists(FString Name);

	void LoadPlayerCalibration();

	UCustomDataStructures::CalibrationData GetSensorCalibration(UCustomDataStructures::RigSensors RigSensor);

	UPROPERTY(EditAnywhere)
		int NumSensorsToCalibrate = 8;

	// How much movement to wait for to verify that player is in fact moving (and the sensor is not merely swaying)
	UPROPERTY(EditAnywhere)
		int MovementThreshold = 25;

	// Must also wait 50 ms before it can be assumed the player is moving
	UPROPERTY(EditAnywhere)
		int TimeThreshold = 50;

private:

	float TimeOne, CurrentPosition, AveragePositionSinceVelocityReverse = 0, CalibrationProgress = 0; 
	const int NumCalibrationReps = 3;
	int NumFramesSinceVelocityReverse = 0, CalibrationReps = 0, MovementStepsDetected = 0, LastPosition = 0;
	bool ActivateCalibration = false, FrameSwitching = false, FinishedCalibration = false, PrintDate = false;

	UCustomDataStructures::CalibrationStates CurrentCalibrationState;

	UCustomDataStructures::CalibrationData FinalCalibrationData = { 0, 0, 0 };

	UCustomDataStructures::CalibrationData Recording[4] = {
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 }
	};

	int Sensor = 0;
	int SensorIndex = 0;

	UPROPERTY(EditAnywhere)
		int SensorsToCalibrate[8];
	

	FString FilePath;
	FString FileContent;
	const FString Extension = L".txt";
	
	FString FirstName = L"Player";
	FString LastName = L"Name";

	FString CurrentSensorReading;

	TArray<FString> ExistingUsers;
	
};
