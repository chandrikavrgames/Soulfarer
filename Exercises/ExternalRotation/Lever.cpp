// Fill out your copyright notice in the Description page of Project Settings.

#include "Lever.h"

#include "Components/InputComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
// Sets default values
ALever::ALever()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//reset velocity list
	for (int i = 0; i < maxVelList; i++) {
		arr_velocities.Add(0);
	}


}

// Called when the game starts or when spawned
void ALever::BeginPlay()
{
	Super::BeginPlay();

	//changing color
	objectMesh = FindComponentByClass<UStaticMeshComponent>();
	auto material = objectMesh->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(material, NULL);
	objectMesh->SetMaterial(0, DynamicMaterial);

	//set world limit
	worldLimits.X = GetActorLocation().Y + localLimits.X;
	worldLimits.Y = GetActorLocation().Y + localLimits.Y;

	
}

// Called every frame
void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	updatePosition(DeltaTime);

}

// Called to bind functionality to input
void ALever::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MouseX", this, &ALever::setMouseDeltaLoc);

}




void ALever::replaceVelocityInArray(float newVelocity) {
	arr_velocities.RemoveAt(0, 1, true); //Pop();
	arr_velocities.Insert(newVelocity, maxVelList - 1);//Push(newVelocity);
}

float ALever::getAverageVelocity() {
	float average = 0;
	for (int i = 0; i < maxVelList; i++) {
		average += abs(arr_velocities[i]);
	}
	average /= maxVelList;
	//UE_LOG(LogTemp, Display, TEXT("average: %f"), average);
	return average;
}

void ALever::zeroVelocityList() {
	for (int i = 0; i < maxVelList; i++) {
		arr_velocities[i] = 0;
	}
}

//void ALever::externalRotation(float value) {
//	DeltaLocation = FVector(0, 0, value);
//}

void ALever::setMouseDeltaLoc(float val) {
	//UE_LOG(LogTemp, Display, TEXT("xvel: %f"), x);
	DeltaLocation = FVector(val, 0, 0);
}

void ALever::updatePosition(float tick) {
	
	setPenaltyColor(tick);

	checkPenalty();

	


	FVector newPos = GetActorLocation();
	newPos += DeltaLocation;
	if (newPos.X <= worldLimits.X) {
		newPos.X = worldLimits.X;

	}
	else if (newPos.X >= worldLimits.Y) {
		newPos.X = worldLimits.Y;

	}
	SetActorLocation(newPos);
}

void ALever::checkPenalty() {
	if (DeltaLocation.Size() > maxDeltaLocationSize) {
		zeroVelocityList();

		penaltyTimeLeft = speedingPenaltyTime;

		DeltaLocation.X *= speedPenalty;
		DeltaLocation.Y *= speedPenalty;
		DeltaLocation.Z *= speedPenalty;

	}
	else {
		replaceVelocityInArray(DeltaLocation.Size());
		setOkayColor();
	}
}

void ALever::setPenaltyColor(float tick) {
	if (penaltyTimeLeft > 0) {
		//if there is, get the ratio
		float penaltyColorBlend = penaltyTimeLeft / speedingPenaltyTime;//.05f + FMath::Cos(penaltyTimeLeft)/2;
		//sets the interpolation value for default -> penalty color
		DynamicMaterial->SetScalarParameterValue(TEXT("PenaltySpeedBlend"), penaltyColorBlend);
		penaltyTimeLeft -= tick;

	}
	else {
		//otherwise, no interpolation
		float penaltyColorBlend = 0;//.05f + FMath::Cos(penaltyTimeLeft)/2;
		DynamicMaterial->SetScalarParameterValue(TEXT("PenaltySpeedBlend"), penaltyColorBlend);

	}
}

void ALever::setOkayColor() {
	float blendOk = getAverageVelocity() / maxDeltaLocationSize;// / maxVelocity.Z;
	DynamicMaterial->SetScalarParameterValue(TEXT("OkaySpeedBlend"), blendOk);
}