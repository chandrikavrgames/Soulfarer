// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Private/Serial.h"
#include "CoreMinimal.h"
#include "CustomDataStructures.h"
#include "GameFramework/Pawn.h"
#include "RigInputController.generated.h"

UCLASS()
class SKYFARER_C_API ARigInputController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARigInputController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	bool IsConnected() { return bIsConnected; }
	void UpdateSensorData();
	UCustomDataStructures::SerialData GetSerialInput(UCustomDataStructures::RigSensors RigSensor);
	void CloseSerialPort();
	float fVelocityMultiplier = 7.0f;
	float FrameLength = 0.0f;
	const float FrameRate = 0.016f;
	TMap<int32, UCustomDataStructures::SerialData> RigDataMap;


	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	USerial * Arduino = nullptr;
	bool bIsConnected = false;
	bool bShouldTick = true;


	void StoreSerialInput(FString Response);

	void ClearSerialData();


	//for getting mouse inputs. For if we somehow find a way to also get positions
	void setMouseInput();

	//functions for setting values just in case we can somehow get positions in here too
	void setMouseX(float x);
	void setMouseY(float y);
	
	float mouseY_velocity;
	float mouseX_velocity;


	void printText();

};
