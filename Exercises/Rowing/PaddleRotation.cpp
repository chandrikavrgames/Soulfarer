// Fill out your copyright notice in the Description page of Project Settings.

#include "PaddleRotation.h"
#include "Components/InputComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "EngineGlobals.h"
#include "Inputs/Rig/RigInputController.h"
#include "Components/AudioComponent.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

// Sets default values
APaddleRotation::APaddleRotation()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APaddleRotation::BeginPlay()
{
	Super::BeginPlay();
	//Paddle = this->FindComponentByClass<UStaticMeshComponent>();
}

// Called every frame
void APaddleRotation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (this->AutoPossessPlayer == EAutoReceiveInput::Player0) {
	//	SerialData = pRigInputManager->GetSerialInput(UCustomDataStructures::FrontLeft);
	//	Paddling(SerialData.Velocities.Z*10.0);
	//}
	//else if (this->AutoPossessPlayer == EAutoReceiveInput::Player1) {
	//	if (ReachedPaddleLimit) {
	//		inc = .5f;
	//		if (RootComponent->K2_GetComponentRotation().Yaw >= 60.0f) {
	//			ReachedPaddleLimit = false;
	//			this->FindComponentByClass<UAudioComponent>()->Play();
	//		}

	//	}
	//	else if (!ReachedPaddleLimit) {
	//		inc = -.5f;
	//		if (RootComponent->K2_GetComponentRotation().Yaw <= -40.0f) {
	//			ReachedPaddleLimit = true;
	//			this->FindComponentByClass<UAudioComponent>()->FadeOut(1.5f, 0.0f);
	//		}

	//	}
	//	RootComponent->AddRelativeRotation(FRotator(0.0f, inc, 0.0f));
	//}

}

// Called to bind functionality to input
void APaddleRotation::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	//check(PlayerInputComponent);
	//PlayerInputComponent->BindAxis("Rowing", this, &APaddleRotation::Paddling);
}

void APaddleRotation::Paddling(float value)
{
	//if ((Controller != NULL) && (value != 0.0f))
	//{
	//	float P = this->FindComponentByClass<UStaticMeshComponent>()->GetComponentRotation().Pitch;
	//	float Y = this->FindComponentByClass<UStaticMeshComponent>()->GetComponentRotation().Yaw;
	//	float R = this->FindComponentByClass<UStaticMeshComponent>()->GetComponentRotation().Roll;
	//	FRotator deltaRot(0.0, 0.0, 0.0);
	//	if (value < 0)
	//	{
	//		if (P > 0)
	//			deltaRot.Add(0.2*value, value, 0.15*value);
	//		else
	//			deltaRot.Add(0.2*value, value, -0.15*value);
	//		if(P>-15 || Y>-82 || R <-32)
	//			this->FindComponentByClass<UStaticMeshComponent>()->AddRelativeRotation(deltaRot);
	//		
	//		if (Y < 0 && ReachedPaddleLimit) {
	//			FHitResult BoatHitResult;
	//			GetAttachParentActor()->K2_SetActorLocation(Waypoints[3 - TotalReps], false, BoatHitResult, true);
	//			TotalReps--;
	//			
	//			if (TotalReps == 0) {
	//				GetAttachParentActor()->K2_SetActorRotation(FRotator::ZeroRotator, true);
	//				TransitionGameState();
	//			}
	//			ReachedPaddleLimit = false;
	//		}
	//		
	//	}
	//	else
	//	{
	//		

	//		if (P > 0)
	//			deltaRot.Add(0.2*value, value, 0.15*value);
	//		else
	//			deltaRot.Add(0.2*value, value, -0.15*value);
	//		if (P < 15 || Y < 75 || R < -32)
	//			this->FindComponentByClass<UStaticMeshComponent>()->AddRelativeRotation(deltaRot);
	//		if (Y > 65)
	//		{
	//			ReachedPaddleLimit = true;
	//		}
	//		
	//	}

		
		
		//UE_LOG(LogTemp, Display, TEXT("%f"), value);
		//print(this->FindComponentByClass<UStaticMeshComponent>()->GetComponentRotation().ToString());
	//}
}


