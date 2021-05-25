// Fill out your copyright notice in the Description page of Project Settings.

#include "GenericRigPawn.h"
#include "Components/InputComponent.h"
#include "Misc/DateTime.h"
#include "State/GameInstanceManager.h"
#include "Inputs/Rig/RigInputController.h"

// Sets default values
AGenericRigPawn::AGenericRigPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

TMap<int32, UCustomDataStructures::CalibrationData> AGenericRigPawn::SensorCalibrations;

// Called when the game starts or when spawned
void AGenericRigPawn::BeginPlay()
{
	Super::BeginPlay();
	UGameInstance* UGame = GetGameInstance();
	UGameManager = static_cast<UGameInstanceManager*>(UGame);
	this->GetComponents<USkeletalMeshComponent>(AttachedMeshes);
	//UGameManager->PossessFirstState();
}

void AGenericRigPawn::remoteTriggerSpiritAction(int state) {
	UGameManager->remoteTriggerSpiritAction(state);
}

void AGenericRigPawn::remoteTriggerSpiritMovement(int movement) {
	UGameManager->remoteTriggerSpiritMovement(movement);
}

// Called every frame 
void AGenericRigPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (pRigInputManager == nullptr) {
		pRigInputManager = UGameManager->pRigInputManager;
	}

}

// Called to bind functionality to input
void AGenericRigPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MouseX", this, &AGenericRigPawn::externalRotation);
	PlayerInputComponent->BindAction("SwitchToRig",IE_Pressed, this, &AGenericRigPawn::UsingRigInput);
	PlayerInputComponent->BindAction("SwitchToMouse", IE_Pressed, this, &AGenericRigPawn::UsingMouseInput);
}

void AGenericRigPawn::TransitionGameState() {
	UGameManager->TransitionGameState();
}

void AGenericRigPawn::RollCredits() {
	UGameManager->EnableCredits();
}

UCustomDataStructures::CalibrationData AGenericRigPawn::GetCalibration(UCustomDataStructures::RigSensors Sensor) {
	UCustomDataStructures::CalibrationData* Payload = SensorCalibrations.Find(Sensor);
	if (Payload != nullptr) {
		return *Payload;
	}
	else return UCustomDataStructures::CalibrationData();
}

float AGenericRigPawn::GetPositionDifference(UCustomDataStructures::RigSensors Sensor) {
	UCustomDataStructures::CalibrationData Calibration = GetCalibration(Sensor);
	return abs(Calibration.EndPositionThreshold[0] - Calibration.StartPositionThreshold[1]);
}

//returns amount that it has moved since the last frame
FVector AGenericRigPawn::CalcDeltaLoc(UCustomDataStructures::RigSensors Sensor)
{
	FVector CurrentRigLoc = SerialData.Positions;
	CurrentTime = FDateTime::UtcNow();

	// When a player messed up a rep and is forced to reset, they must return their arm to the starting position
	//if (CheckForExerciseReset) { 
	//	UCustomDataStructures::CalibrationData Calibration = GetCalibration(Sensor);
	//	UE_LOG(LogTemp, Warning, TEXT("(%f : %f)"), CurrentRigLoc.Z, Calibration.StartPosition);
	//	//if near start position
	//	if (abs(CurrentRigLoc.Z-Calibration.StartPosition) < 200.0) {
	//		//you have to hold for 120 frames to finish a rep 
	//		FramesReset++;
	//		if (FramesReset > 120) {
	//			CheckForExerciseReset = false;
	//			FramesReset = 0;
	//		}
	//		else {
	//			//otherwise, it should not move at al
	//			return FVector::ZeroVector;
	//		}
	//	}
	//	else {
	//		return FVector::ZeroVector;
	//	}
	//}



	// The goal of this check is to not count when the sensor is just swaying on its own without human input and sporadically
	//  sending data every few frames (whereas human input would be consistent frame-to-frame)
	// In this case, the delta location will just be zero so the object doesn't move at all
	//if ((CurrentTime-TimeOfLastUpdate).GetTotalMilliseconds() > 40) PreviousRigLocation = CurrentRigLoc;
	
	//UE_LOG(LogTemp, Display, TEXT("%f, %f"), PreviousRigLocation.Z, CurrentRigLoc.Z);
	DeltaLocation = CurrentRigLoc -	PreviousRigLocation;

	// Case where the player is not moving in sync with the current direction of the exercise
	//if (DeltaLocation.Z*CurrentDirection < 0) {
	//	FramesInDirection++;
	//	if (FramesInDirection > 30) {
	//		FramesFrozen++;
	//		return FVector::ZeroVector;
	//	}
	//}
	//else {
	//	FramesFrozen = 0;
	//}

	//if (DeltaLocation.Size() < 3) {
	//	// Barely any movement since last frame
	//	FramesFrozen++;
	//}
	//else {
	//	FramesFrozen = 0;
	//}

	//when you initialy boot up the game, the rig sends infinity
	//10 is an arbitrary treshhold so that if something happens and the inputs become huge, just disregard it
	//if (DeltaLocation.Size() > 10) DeltaLocation = FVector::ZeroVector;

	PreviousRigLocation = CurrentRigLoc;
	TimeOfLastUpdate = CurrentTime;
	return DeltaLocation;
}

void AGenericRigPawn::IsPlayerInSync(float ExerciseDirection) {
	if (CurrentDirection > 0) {
		RepUnsync = true;
		FramesUnsync++;
	}
	else {
		RepUnsync = false;
		FramesUnsync = 0;
	}
}

UCustomDataStructures::SerialData AGenericRigPawn::UpdateSerialData(UCustomDataStructures::RigSensors Sensor) {
	UCustomDataStructures::SerialData RawSerialData = pRigInputManager->GetSerialInput(Sensor);
	if (IsCalibration) return RawSerialData;
	UCustomDataStructures::CalibrationData Calibration = GetCalibration(Sensor);
	//UE_LOG(LogTemp, Display, TEXT("%f: %f: %f"), Calibration.StartPosition, Calibration.EndPosition, abs(Calibration.StartPosition - Calibration.EndPosition));
	//UE_LOG(LogTemp, Display, TEXT("before: %f"), RawSerialData.Positions.Z);
	//RawSerialData.Positions.Z = abs(RawSerialData.Positions.Z - Calibration.StartPosition) / (abs(Calibration.StartPosition - Calibration.EndPosition));
	RawSerialData.Positions.X = abs(RawSerialData.Positions.X) / (abs(640));
	/*if (RawSerialData.Positions.Z - Calibration.StartPosition > 0) RawSerialData.Positions.Z = 0;
	else if (RawSerialData.Positions.Z > 1) RawSerialData.Positions.Z = 1;*/
	if (RawSerialData.Positions.X  < 0) RawSerialData.Positions.X = 0;
	else if (RawSerialData.Positions.X > 1) RawSerialData.Positions.X = 1;
	//UE_LOG(LogTemp, Display, TEXT("after: %f"), RawSerialData.Positions.Z);
	return RawSerialData;
}

void AGenericRigPawn::UsingRigInput() {
	UsingRig = true;
}

void AGenericRigPawn::UsingMouseInput() {
	UsingRig = false;
}


void AGenericRigPawn::externalRotation(float value) {
	if (!UsingRig) {
		DeltaLocation = FVector(0, 0, value);
	}
}