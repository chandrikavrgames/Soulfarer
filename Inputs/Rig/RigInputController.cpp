// Fill out your copyright notice in the Description page of Project Settings.

#include "RigInputController.h"

#include "Runtime/Engine/Classes/Components/InputComponent.h"

// Sets default values
ARigInputController::ARigInputController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RigDataMap.Reserve(9);
	for (int a = 0; a < 9; a++) {
		RigDataMap.Add(a, UCustomDataStructures::EmptyData);
	}
	//for (auto It = RigDataMap.CreateConstIterator(); It; ++It)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Constructor: (%d, %f, %f, %f, %f, %f, %f)\n"),
	//		It.Key(),   // same as It->Key
	//		It.Value().Positions.X,
	//		It.Value().Positions.Y,
	//		It.Value().Positions.Z,
	//		It.Value().Velocities.X,
	//		It.Value().Velocities.Y,
	//		It.Value().Velocities.Z
	//	);
	//}

}

// Called when the game starts or when spawned
void ARigInputController::BeginPlay()
{
	Super::BeginPlay();
	//Arduino = USerial::OpenComPort(bIsConnected, 8);
	if (bIsConnected) {
		Arduino->Flush();
		UpdateSensorData();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Failed to open port"));
	}

	//for (auto It = RigDataMap.CreateConstIterator(); It; ++It)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Begin Play: (%d, %f, %f, %f, %f, %f, %f)\n"),
	//		It.Key(),   // same as It->Key
	//		It.Value().Positions.X,
	//		It.Value().Positions.Y,
	//		It.Value().Positions.Z,
	//		It.Value().Velocities.X,
	//		It.Value().Velocities.Y,
	//		It.Value().Velocities.Z
	//	);
	//}

	//UCustomDataStructures::SerialData OMFG = GetSerialInput(UCustomDataStructures::RigSensors(0));
	//UE_LOG(LogTemp, Warning, TEXT("(%f, %f, %f, %f, %f, %f)\n"),
	//	OMFG.Positions.X,
	//	OMFG.Positions.Y,
	//	OMFG.Positions.Z,
	//	OMFG.Velocities.X,
	//	OMFG.Velocities.Y,
	//	OMFG.Velocities.Z
	//);

}

// Called to bind functionality to input
//JESS: This is being called, but it doesn't bind the inputs
void ARigInputController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//get mouse inputs. From Project Settings
	check(PlayerInputComponent);
	UE_LOG(LogTemp, Display, TEXT("Calling Rig InputComponent"));
	PlayerInputComponent->BindAxis("MouseX", this, &ARigInputController::setMouseX);
	PlayerInputComponent->BindAxis("TestMouseY", this, &ARigInputController::setMouseY);
	//PlayerInputComponent->BindAxis("TestInput", this, &ARigInputController::printText);
	UE_LOG(LogTemp, Display, TEXT("Finished binding input axis?"));
}

void ARigInputController::CloseSerialPort() {
	//Arduino->Close();
	RigDataMap.Empty(8);
}

UFUNCTION()
void ARigInputController::UpdateSensorData() {
	if (bIsConnected)
	{
		bool bSuccessfulRead = true;
		FString Response;
		
		int numRigSensors = 8; //Total size of RigSensors is 9, but this includes the mouse, and we don't want that in this loop. 
		for (int i = 0; i <= numRigSensors; i++) {

			Response = Arduino->Readln(bSuccessfulRead);
			if (bSuccessfulRead) {
				StoreSerialInput(Response);
			}
			//for (auto It = RigDataMap.CreateConstIterator(); It; ++It)
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("(%d, %f, %f, %f, %f, %f, %f)\n"),
			//		It.Key(),   // same as It->Key
			//		It.Value().Positions.X,
			//		It.Value().Positions.Y,
			//		It.Value().Positions.Z,
			//		It.Value().Velocities.X,
			//		It.Value().Velocities.Y,
			//		It.Value().Velocities.Z
			//	);
			//}

		}

		
	}

	//set mouse

	setMouseInput();


}

void ARigInputController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (FrameLength > FrameRate)
	{
		FrameLength = 0.0f;
		ClearSerialData();
		UpdateSensorData();
	}
	else {
		FrameLength += DeltaTime;
	}
}

void ARigInputController::ClearSerialData() {
	for (auto It = RigDataMap.CreateConstIterator(); It; ++It) {
		UCustomDataStructures::SerialData ResetVelocity = It.Value();
		ResetVelocity.Velocities.Set(0.0f, 0.0f, 0.0f);
		RigDataMap.Emplace(It.Key(), ResetVelocity);
	}
}

void ARigInputController::StoreSerialInput(FString Response) {

	UCustomDataStructures::SerialData Payload;

	FString SensorIndex;
	FString Velocities;
	FString Positions;
	FString SerialPayload;
	FString SearchOp = ":";

	Response.Split(SearchOp, &SensorIndex, &SerialPayload);
	SerialPayload.Split(SearchOp, &Positions, &Velocities);

	Payload.Positions.InitFromString(Positions);
	Payload.Velocities.InitFromString(Velocities);

	RigDataMap.Emplace(FCString::Atoi(*SensorIndex), Payload);
}

UCustomDataStructures::SerialData ARigInputController::GetSerialInput(UCustomDataStructures::RigSensors RigSensor) {
	//for (auto It = RigDataMap.CreateConstIterator(); It; ++It)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("(%d, %f, %f, %f, %f, %f, %f)\n"),
	//		It.Key(),   // same as It->Key
	//		It.Value().Positions.X,
	//		It.Value().Positions.Y,
	//		It.Value().Positions.Z,
	//		It.Value().Velocities.X,
	//		It.Value().Velocities.Y,
	//		It.Value().Velocities.Z
	//	);
	//}
	return *RigDataMap.Find(RigSensor);
}

void ARigInputController::setMouseInput() {
	UCustomDataStructures::SerialData Payload;
	
	//the way we get mouse inputs doesn't give position. 
	Payload.Positions = FVector(0, 0, 0);
	Payload.Velocities = FVector(mouseX_velocity, mouseY_velocity, 0);
	//UE_LOG(LogTemp, Display, TEXT("Calling mouse: %f X , %f Y"), mouseX_velocity, mouseY_velocity);
	RigDataMap.Emplace(UCustomDataStructures::RigSensors::Mouse, Payload);
}

void ARigInputController::setMouseX(float x) {
	mouseX_velocity = x;

}

void ARigInputController::setMouseY(float y) {
	mouseY_velocity = y;
	//UE_LOG(LogTemp, Display, TEXT("Y: %f"), y);
}

void ARigInputController::printText() {
	UE_LOG(LogTemp, Display, TEXT("pressed a key"));
}