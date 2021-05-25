// Fill out your copyright notice in the Description page of Project Settings.

#include "Exercises/Dumbell/EnergyBall.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Inputs/Rig/RigInputController.h"


// Sets default values
AEnergyBall::AEnergyBall()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere Static Mesh"));

	// This is Component Specific. Just wanted to show you that you can access it here too.
	SphereStaticMesh->bReceivesDecals = false;
	SphereStaticMesh->bCastDynamicShadow = true;
	SphereStaticMesh->SetOnlyOwnerSee(false);
	SphereStaticMesh->SetOwnerNoSee(false);

	ExerciseSensors = {
		UCustomDataStructures::DownLeft,
		UCustomDataStructures::DownRight
	};
}

// Called when the game starts or when spawned
void AEnergyBall::BeginPlay()
{
	
	Super::BeginPlay();
	//startLoc = SphereStaticMesh->K2_GetComponentLocation();
	startLoc = K2_GetActorLocation();
	//startLoc.Z += 7050;
	auto material = SphereStaticMesh->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(material, NULL);
	SphereStaticMesh->SetMaterial(0, DynamicMaterial);
	
}



// Called every frame
void AEnergyBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (this->AutoPossessPlayer == EAutoReceiveInput::Player0) {
		
		SetYValueRig();
		
		float changeY = abs(CalcDeltaLoc(ExerciseSensors[0]).Z)/10;
		if (changeY > 50) changeY = 0;
		//UE_LOG(LogTemp, Warning, TEXT("(%f)"), (abs(GetPositionDifference(ExerciseSensors[0])) / 4));
		//check if ball is moving too fast
		if (abs(lastY) > GetCalibration(ExerciseSensors[0]).ZVelocityThreshold[1])
		{
			penaltyTime = blendDuration;
		}
		//reset pos if past 350 units
		if (this->K2_GetActorLocation().Z-7100 > (abs(GetPositionDifference(ExerciseSensors[0]))/4-10))
		{
			this->SetActorLocation(startLoc);
			TotalReps--;
			//if (TotalReps == 0)
				//TransitionGameState();
		}
		//start to float away past 190 units
		else if (this->K2_GetActorLocation().Z-7100 > 0.9*2*((4 * abs(GetPositionDifference(ExerciseSensors[0])))+20))
		{
			/*FVector deltaLoc(0, 0, DeltaTime * 200);
			SphereStaticMesh->AddRelativeLocation(deltaLoc);*/

		}
		else
		{
			FVector deltaLoc(0, 0, changeY*3);
			AddActorLocalOffset(deltaLoc);
		}

		float blend = 0;
		if (penaltyTime > 0)
		{
			penaltyTime -= DeltaTime;
			blend = penaltyTime / blendDuration;
		}
		DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), blend);
	}
}

// Called to bind functionality to input
void AEnergyBall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//check(PlayerInputComponent);
	//PlayerInputComponent->BindAxis("MouseX", this, &AEnergyBall::setMouseX);
	//PlayerInputComponent->BindAxis("MouseY", this, &AEnergyBall::setMouseY);

}

void AEnergyBall::setMouseX(float x)
{
	lastX = x;
	//UE_LOG(LogTemp, Display, TEXT("X: %f"), x);
}

void AEnergyBall::setMouseY(float y)
{
	lastY = y;
	//UE_LOG(LogTemp, Display, TEXT("Y: %f"), y);
}

void AEnergyBall::SetYValueRig()
{
	 SerialData = pRigInputManager->GetSerialInput(UCustomDataStructures::DownLeft);
	 lastY = abs(SerialData.Velocities.Z/1000);
}

