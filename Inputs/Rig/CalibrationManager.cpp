// Fill out your copyright notice in the Description page of Project Settings.

#include "CalibrationManager.h"
#include "Components/InputComponent.h"
#include "Misc/DateTime.h"
#include "FileHelper.h"
#include "HAL/FileManager.h"
#include "Paths.h"
#include "Inputs/Rig/RigInputController.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "PlatformFilemanager.h" 

// Sets default values
ACalibrationManager::ACalibrationManager()
{
	ExistingUsers = GetAllPlayerNames();

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	{
		// Adding default calibration values to SensorCalibrations Map
		/*SensorCalibrations.Add(1, { 1, 6, 0, 1114.233398, 816.944519, 0.115730, {0.9*1114.233398, 1.1*1114.233398 }, {0.9*816.944519 , 1.1*816.944519 }, {0.9*0.115730, 1.1*0.115730 } });
		SensorCalibrations.Add(0, { 1, 6, 0, 1049.822266, 881.411133, 0.145935,{ 0.9*1049.822266, 1.1*1049.822266 },{ 0.9*881.411133 , 1.1*881.411133 },{ 0.9*0.145935, 1.1*0.145935 } });
		SensorCalibrations.Add(2, { 1, 6, 0, 1169.607422, 963.103699, 0.146995,{ 0.9*1169.607422, 1.1*1169.607422 },{ 0.9*963.103699 , 1.1*963.103699 },{ 0.9*0.146995, 1.1*0.146995 } });
		SensorCalibrations.Add(3, { 1, 6, 0, 1302.235840, 1164.501343, 0.143551,{ 0.9*1302.235840, 1.1*1302.235840 },{ 0.9*1164.501343 , 1.1*1164.501343 },{ 0.9*0.143551, 1.1*0.143551 } });
		SensorCalibrations.Add(4, { 1, 6, 0, 1315.911987, 1177.400513, 0.065371,{ 0.9*1315.911987, 1.1*1315.911987 },{ 0.9*1177.400513 , 1.1*1177.400513 },{ 0.9*0.065371, 1.1*0.065371 } });
		SensorCalibrations.Add(5, { 1, 6, 0, 1187.004028, 1002.433533, 0.088702,{ 0.9*1187.004028, 1.1*1187.004028 },{ 0.9*1002.433533 , 1.1*1002.433533 },{ 0.9*0.088702, 1.1*0.088702 } });
		SensorCalibrations.Add(6, { 1, 6, 0, 1298.768066, 1119.911255, 0.202717,{ 0.9*1298.768066, 1.1*1298.768066 },{ 0.9*1119.911255 , 1.1*1119.911255 },{ 0.9*0.202717, 1.1*0.202717 } });
		SensorCalibrations.Add(7, { 1, 6, 0, 1318.922729, 1016.670532, 0.138155,{ 0.9*1318.922729, 1.1*1318.922729 },{ 0.9*1016.670532 , 1.1*1016.670532 },{ 0.9*0.138155, 1.1*0.138155 } });*/

		//PreviousRigLocation = { 0,0, };

	}



	
	/* 
	Taken on December 8, 2019
		(1: 826.066650, 617.133362, 0.444581) 
		(2: 1049.822266, 881.411133, 0.145935) 
		(3: 1169.607422, 963.103699, 0.146995) 
		(4: 1302.235840, 1164.501343, 0.143551) 
		(5: 1315.911987, 1177.400513, 0.065371) 
		(6: 1187.004028, 1002.433533, 0.088702) 
		(7: 1298.768066, 1119.911255, 0.202717) 
		(8: 1318.922729, 1016.670532, 0.138155) 
	*/
}

ACalibrationManager::~ACalibrationManager() {
}

// Called when the game starts or when spawned
void ACalibrationManager::BeginPlay()
{
	Super::BeginPlay();
	//BeginCalibration();
	
}

int ACalibrationManager::DecrementCalibrationSensor() {
	if(SensorIndex > 0)
		SensorIndex--;
	Sensor = SensorsToCalibrate[SensorIndex];
	return Sensor;
}

bool ACalibrationManager::FinishedCurrentCalibration() {
	return FinishedCalibration;
}

float ACalibrationManager::GetCalibrationProgress() {
	return CalibrationProgress;
}


FString ACalibrationManager::peanutbutter()
{
	/*if (FinishedCalibration) {
		FinishedCalibration = false;
		
	}*/
	return CurrentSensorReading;
	//return FString("CALIBRATE!");
}

// Find all calibration text files in this folder
TArray<FString> ACalibrationManager::GetAllPlayerNames() {

	TArray<FString> FoundFiles;

	IFileManager*_FileManager = &IFileManager::Get();
	_FileManager->FindFiles(FoundFiles, *FPaths::ProjectSavedDir(), *Extension);

	TArray<FString>* ReturningPlayers = new TArray<FString>();
	ReturningPlayers->Init(L"Player Name", FoundFiles.Num());

	for (int i = 0; i < FoundFiles.Num(); i++) {
		FString UnderscoreDelim = L"_";
		FString FName;
		FString StringParse;
		FString StringParseTwo;
		FString LName;

		FoundFiles[i].Split(UnderscoreDelim, &FName, &StringParse);
		StringParse.Split(UnderscoreDelim, &LName, &StringParseTwo);

		(*ReturningPlayers)[i] = FName + " " + LName;
	}

	return *ReturningPlayers;

}

// Called every frame
void ACalibrationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ActivateCalibration) {
		Calibrate(DeltaTime);
	}
}

void ACalibrationManager::Calibrate(float DeltaTime) {

	//ARigInputController::SerialData SerialData = pRigInputManager->GetSerialInput(Sensor);
	IsCalibration = true;

	if (CurrentCalibrationState == UCustomDataStructures::CalibrationStates::FullyExtended) {

		//UE_LOG(LogTemp, Warning, TEXT("FULLY EXTENDED"));
		
		// The rig is full extended. Take a reading
		SerialData = UpdateSerialData(UCustomDataStructures::RigSensors(Sensor));
		LastPosition = SerialData.Positions[2];

		// Now start the timer and wait for movement to know that the player is beginning a rep
		TimeOne = 0;

		//if (CheckForMovement(true, DeltaTime)) {
			// Player is verified to be relaxing their stretch
		CurrentCalibrationState = UCustomDataStructures::CalibrationStates::Relaxing;
		
		//}
	}
	else if (CurrentCalibrationState == UCustomDataStructures::CalibrationStates::Relaxing) {
		//UE_LOG(LogTemp, Warning, TEXT("RELAXING"));
		TimeOne += DeltaTime;
		CurrentPosition = CheckForVelocityReversal(-1.0);
		if (CurrentPosition != -1.0f) {
			CurrentCalibrationState = UCustomDataStructures::CalibrationStates::FullyRelaxed;
		}
	} 
	else if (CurrentCalibrationState == UCustomDataStructures::CalibrationStates::FullyRelaxed) {
		//UE_LOG(LogTemp, Warning, TEXT("FULLY RELAXED"));
		Recording[CalibrationReps].StartPosition = CurrentPosition;
		Recording[CalibrationReps].AverageVelocity = (abs(Recording[CalibrationReps].EndPosition - Recording[CalibrationReps].StartPosition) / (0.00001f + (TimeOne)));
		TimeOne = 0;

		//if (CheckForMovement(false, DeltaTime)) {
			// Player is verified to be relaxing their stretch
		CurrentCalibrationState = UCustomDataStructures::CalibrationStates::Extending;

		CalibrationProgress = (float)CalibrationReps / (float)NumCalibrationReps + 0.5 * 1 / NumCalibrationReps;

		//}
	} 
	else if (CurrentCalibrationState == UCustomDataStructures::CalibrationStates::Extending) {
		//UE_LOG(LogTemp, Warning, TEXT("EXTENDING"));
		TimeOne += DeltaTime;
		CurrentPosition = CheckForVelocityReversal(1.0);
		if (CurrentPosition != -1.0f) {
			CurrentCalibrationState = UCustomDataStructures::CalibrationStates::FullyExtended;
			Recording[CalibrationReps].EndPosition = CurrentPosition;
			Recording[CalibrationReps].AverageVelocity += (abs(CurrentPosition - Recording[CalibrationReps].StartPosition) / (0.00001f + (TimeOne)));
			Recording[CalibrationReps].AverageVelocity /= 2;
			CalibrationReps++;
			CalibrationProgress = (float)CalibrationReps / (float)NumCalibrationReps;
		}
	}

	if (CalibrationReps == NumCalibrationReps) {
		for (int i = 0; i < 3; i++) {
			UE_LOG(LogTemp, Warning, TEXT("%f : %f"), Recording[i].StartPosition, Recording[i].EndPosition);
			FinalCalibrationData.StartPosition += Recording[i].StartPosition;
			FinalCalibrationData.EndPosition += Recording[i].EndPosition;
			FinalCalibrationData.AverageVelocity += Recording[i].AverageVelocity;
		}

		FinalCalibrationData.StartPosition /= 3;
		FinalCalibrationData.EndPosition /= 3;
		FinalCalibrationData.AverageVelocity /= 3000;

		FinalCalibrationData.ZVelocityThreshold = { FinalCalibrationData.AverageVelocity*0.9f, FinalCalibrationData.AverageVelocity*1.1f };
		FinalCalibrationData.StartPositionThreshold = { FinalCalibrationData.StartPosition*0.9f, FinalCalibrationData.StartPosition*1.1f };
		FinalCalibrationData.EndPositionThreshold = { FinalCalibrationData.EndPosition*0.9f, FinalCalibrationData.EndPosition*1.1f };

		CalibrationReps = 0;

		UE_LOG(LogTemp, Warning, TEXT("(%d, %f, %f, %f)"), Sensor, FinalCalibrationData.StartPosition, FinalCalibrationData.EndPosition, FinalCalibrationData.AverageVelocity);
		CurrentSensorReading = FString::Printf(TEXT("%d, %f, %f, %f"), Sensor, FinalCalibrationData.StartPosition, FinalCalibrationData.EndPosition, FinalCalibrationData.AverageVelocity);

		AGenericRigPawn::SensorCalibrations.Emplace(Sensor, FinalCalibrationData);
		
		// Placeholder value, must be filled in by UI
		float ResistanceBand = 0.0f;
		float Extension = 6.0f;
		float Angle = 0.0f;

		FIntVector RigSettings(ResistanceBand, Extension, Angle);

		// Need to make sure FirstName and LastName are set by this point


		FilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() + (L"/" + FirstName + L"_" + LastName + L"_DefaultCalibration.txt"));

		if (!PrintDate) {
			FDateTime CurrentTime;
			FileContent = CurrentTime.ToString() + "\n";
			FFileHelper::SaveStringToFile(FileContent, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
			PrintDate = true;
		}

		FileContent = (FString::Printf(TEXT("%d:(X=%d, Y=%d, Z=%d):(X=%f, Y=%f, Z=%f) \n"), Sensor, RigSettings.X, RigSettings.Y, RigSettings.Z, FinalCalibrationData.StartPosition, FinalCalibrationData.EndPosition, FinalCalibrationData.AverageVelocity));
		FFileHelper::SaveStringToFile(FileContent, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);

		SensorIndex++;
		FinishedCalibration = true;

		for (int i = 0; i < 3; i++) {
			Recording[i].AverageVelocity = 0;
		}
		FinalCalibrationData.StartPosition = 0;
		FinalCalibrationData.EndPosition = 0;
		FinalCalibrationData.AverageVelocity = 0;

		if (SensorIndex == NumSensorsToCalibrate) {
			ReadyToPlay();
		}

		ActivateCalibration = false;

	}

}

bool ACalibrationManager::CheckIfPlayerExists(FString Name) {
	return (ExistingUsers.Find(Name) != INDEX_NONE);
}

UCustomDataStructures::CalibrationData ACalibrationManager::GetSensorCalibration(UCustomDataStructures::RigSensors RigSensor) {
	return *SensorCalibrations.Find(RigSensor);
}

void ACalibrationManager::LoadPlayerCalibration() {
	FilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() + (L"/" + FirstName + L"_" + LastName + L"_DefaultCalibration.txt"));

	//Attempting to load file
	FString result;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();
	FFileHelper::LoadFileToString(result, *FilePath);

	if (!result.IsEmpty()) {

		// Array of file strings
		TArray<FString> SensorCalibrationStrings;

		// Read string into array of string, one sensor calibration per line
		int NumCalibrations = result.ParseIntoArrayLines(SensorCalibrationStrings);

		FDateTime TimeStamp;
		FString SensorNum;
		FString StringParse;
		FString ResistanceBand;
		FString SensorValues;
		FString Delimiter = L":";
		for (int i = 0; i < NumCalibrations; i++) {

			if (!FDateTime::Parse(SensorCalibrationStrings[i], TimeStamp)) {

				// If this line is a timestamp
				SensorCalibrationStrings[i].Split(Delimiter, &SensorNum, &StringParse);
				int SensorNumber = FCString::Atoi(*SensorNum);

				StringParse.Split(Delimiter, &ResistanceBand, &SensorValues);
				FVector RigSettings;
				RigSettings.InitFromString(ResistanceBand);

				FVector SensorCalibrationValues;
				SensorCalibrationValues.InitFromString(SensorValues);

				UCustomDataStructures::CalibrationData CalibrationToEmplace =

				{
					(int)RigSettings.X, (int)RigSettings.Y, (int)RigSettings.Z,
					SensorCalibrationValues[0], SensorCalibrationValues[1], SensorCalibrationValues[2],
					{ 0.9f*SensorCalibrationValues[0], 1.1f*SensorCalibrationValues[0] },
					{ 0.9f*SensorCalibrationValues[1] , 1.1f*SensorCalibrationValues[1] },
					{ 0.9f*SensorCalibrationValues[2], 1.1f*SensorCalibrationValues[2] }
				};

				SensorCalibrations.Add(SensorNumber, CalibrationToEmplace);

				//UE_LOG(LogTemp, Warning, TEXT("Calibration Exists!: %d, %f"), int(RigSettings.Y), SensorCalibrationValues[0]);

				PreviousRigLocation = { 0,0,SensorCalibrationValues[1] };
			
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Calibration Exists from Time: %s"), *SensorCalibrationStrings[i]);
			}
		}

	}
}

bool ACalibrationManager::SetName(FString Name) {
	FString UnderscoreDelim = L" ";
	Name.Split(UnderscoreDelim, &FirstName, &LastName);
	if (CheckIfPlayerExists(Name)) {
		LoadPlayerCalibration();
		return true;
	}
	return false;
}

FString ACalibrationManager::ShowUser(FString FullName) {
	return FullName;
}

float ACalibrationManager::CheckForVelocityReversal(float Direction) {
	// Now wait as the player moves from max extension to min extension
	// This moment is when the velocities go from negative to positive
	// We should wait a few frames to ensure that the direction of motion has actually changed

	float CurrentVelocity; 

	// Wait for the velocity to switch directions AND wait at least 5 frames of this new shift
	// before recording the begin position

	SerialData = UpdateSerialData(UCustomDataStructures::RigSensors(Sensor));
	CurrentVelocity = SerialData.Velocities[2];
	//UE_LOG(LogTemp, Warning, TEXT("(%f)"), SerialData.Velocities[2]);

	//UE_LOG(LogTemp, Warning, TEXT("(%f, %f)"), CurrentVelocity, Direction);

	// If this product is less than 0 then they are opposite signs
	if (CurrentVelocity*Direction < 0) {
		float ZPosition = SerialData.Positions.Z;
		NumFramesSinceVelocityReverse++;
		AveragePositionSinceVelocityReverse += ZPosition;
		//UE_LOG(LogTemp, Warning, TEXT("(%f)"), AveragePositionSinceVelocityReverse);
		if (NumFramesSinceVelocityReverse >= 20) {
			NumFramesSinceVelocityReverse = 0;
			float SpeedDuringVelocityReverse = AveragePositionSinceVelocityReverse / 20.0f;
			AveragePositionSinceVelocityReverse = 0;
			return SpeedDuringVelocityReverse;		
		}
	}

	return -1.0f;
	//return SerialData.Positions[2];
}

bool ACalibrationManager::CheckForMovement(bool isRelaxing, float DeltaTime) {
	SerialData = UpdateSerialData(UCustomDataStructures::RigSensors(Sensor));
	int NewMovement = (SerialData.Positions[2] - LastPosition);
	
	if ((NewMovement < 0 && isRelaxing) || (NewMovement > 0 && !isRelaxing)) {
		MovementStepsDetected += (NewMovement);
		TimeOne += DeltaTime;
		if (abs(MovementStepsDetected) > MovementThreshold && TimeOne > TimeThreshold) {
			MovementStepsDetected = 0;
			TimeOne = 0;
			return true;
		}
	}

	return false;
}

int ACalibrationManager::GetSensorNum() {
	return SensorIndex;
}

ECalibrationCounter ACalibrationManager::BeginCalibration() {
	if (SensorIndex < NumSensorsToCalibrate) {
		Sensor = SensorsToCalibrate[SensorIndex];
		UE_LOG(LogTemp, Warning, TEXT("SensorIndex %d"), SensorIndex);
		UE_LOG(LogTemp, Warning, TEXT("Sensor %d"), Sensor);
		CurrentCalibrationState = UCustomDataStructures::CalibrationStates::FullyExtended;
		ActivateCalibration = true;
	 
		return ECalibrationCounter::Calibrating;
	}
	else
	{
		TransitionGameState();
		return ECalibrationCounter::DoneCalibrating;
	}
}

// Called to bind functionality to input
void ACalibrationManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("BeginCalibration", IE_Pressed, this, &ACalibrationManager::BeginCalibration);

}

