// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "CustomDataStructures.generated.h"

/**
 * 
 */

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EGameStates : uint8
{
	Warmup	UMETA(DisplayName = "Warmup"),
	Calibration UMETA(DisplayName = "Calibration"),
	ExternalRotation UMETA(DisplayName = "ExternalRotation"),
	Rowing UMETA(DisplayName = "Rowing"),
	Pulldown UMETA(DisplayName = "Pulldown"),
	Dumbbell UMETA(DisplayName = "Dumbbell"),
	Spirit UMETA(DisplayName = "Spirit"),
	Cooldown UMETA(DisplayName = "Cooldown")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ECalibrationCounter : uint8
{
	Calibrating	UMETA(DisplayName = "Calibrating"),
	LastCalibration UMETA(DisplayName = "LastCalibration"),
	DoneCalibrating UMETA(DisplayName = "DoneCalibrating")
};


UCLASS()
class SKYFARER_C_API UCustomDataStructures : public UObject
{
	GENERATED_BODY()

public:

	struct CalibrationData {
		int Resistance;
		int ArmExtension; // minimum 2 holes, normally 6
		int ArmRotation; // -2, -1, 0, 1, or 2
		float StartPosition;
		float EndPosition;
		float AverageVelocity;
		FVector2D ZVelocityThreshold;
		FVector2D StartPositionThreshold;
		FVector2D EndPositionThreshold;
	};

	enum CalibrationStates {
		FullyExtended,
		Relaxing,
		FullyRelaxed,
		Extending
	};

	enum RigSensors {
		UpperLeft,
		MidLeft,
		DownLeft,
		FrontLeft,
		FrontRight,
		DownRight,
		MidRight,
		UpperRight,
		Mouse
	};

	struct SerialData {
		FVector Positions;
		FVector Velocities;
	};

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	//EGameStates GameStates;

	static const SerialData EmptyData;

};
