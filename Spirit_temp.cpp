// Fill out your copyright notice in the Description page of Project Settings.

#include "Spirit_temp.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/Character.h"
#include "Components/AudioComponent.h"

// Sets default values
ASpirit_temp::ASpirit_temp()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpirit_temp::BeginPlay()
{
	Super::BeginPlay();

	//changing color
	objectMesh = FindComponentByClass<USkeletalMeshComponent>();
	auto material = objectMesh->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(material, NULL);
	objectMesh->SetMaterial(0, DynamicMaterial);

	if (DynamicMaterial != nullptr) {
		DynamicMaterial->SetVectorParameterValue("SpiritPrimaryColor", spiritColor);
	}
	SplineComponent = FindComponentByClass<USplineComponent>();
	float totalLength = SplineComponent->GetSplineLength();
	float currentLength = 0;
	float sampleLength = 1; //Sample the spline every "sampleLength" units
	int splinePointCount = 0;
	splinePointer = 0;


	while (currentLength < totalLength) {
		//Get Transform from current location
		FTransform splinePointTransform = SplineComponent->GetTransformAtDistanceAlongSpline(currentLength, ESplineCoordinateSpace::World);
		//Set boat's location and rotation
		pathPointRotation[splinePointCount] = splinePointTransform.GetRotation();
		pathPointLocation[splinePointCount] = splinePointTransform.GetLocation();

		currentLength += sampleLength;//increase "currentLength" for the next sample
		splinePointCount += 1;
	}

	totalSplinePoints = splinePointCount;
	
}

void ASpirit_temp::TriggerSpiritAction(int state) {
	switch (state)
	{
	case 1:
		spiritState = state;
	default:
		actionState = 1;
		RiseSpeed *= 1.5;
		break;
	}
	
}

// Called every frame
void ASpirit_temp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	bool isPossessed = (this->AutoPossessPlayer == EAutoReceiveInput::Player2) ? true : false;

	if (isPossessed || actionState > 0) {
		FVector rotation;
		if (activated) {
			rotation = activatedSpinSpeed * DeltaTime;
			if (DynamicMaterial != nullptr) {

				DynamicMaterial->SetScalarParameterValue("SpiritSpecularity", 1);
			}
		}
		else {
			rotation = spinSpeed * DeltaTime;
			if (DynamicMaterial != nullptr) {

				DynamicMaterial->SetScalarParameterValue("SpiritSpecularity", 0);
			}
		}

		AddActorLocalRotation(FRotator(rotation.X, rotation.Y, rotation.Z));
		
		//state0 =HitPeak & end , state1 = end, state2 = HitPeak, state3 = init
		FVector Delta = { 0.0f, 0.0f, .75f };
		/*if (AttachedMeshes[0]->K2_GetComponentLocation().Z >= RiseDistance && spiritState == 3)
		{
			spiritState--;
		}

		if (AttachedMeshes[0]->K2_GetComponentLocation().Z < RiseDistance && spiritState == 3) {
			Delta.Z = RiseSpeed * DeltaTime;
			this->FindComponentByClass<UAudioComponent>()->Play();
		}*/

		if (AttachedMeshes[0]->K2_GetComponentLocation().Z <= SinkDistance) {
			this->FindComponentByClass<UAudioComponent>()->Stop();
			
			actionState = 0;
			RiseDistance = 350.0f;

			if (isPossessed) TransitionGameState();
			else RiseSpeed *= 2 / 3;
			
		}
		switch(spiritState) {
			case 0:
				RollCredits();
				this->AutoPossessPlayer = EAutoReceiveInput::Disabled;
				break;
			case 1:
				RiseDistance = 2000.f;
				this->FindComponentByClass<UAudioComponent>()->Play();
				break;
			case 2:
				Delta.Z = -1 * (RiseSpeed * DeltaTime);
				break;
			case 3:
				if (AttachedMeshes[0]->K2_GetComponentLocation().Z >= RiseDistance)
					spiritState--;
				else {
					Delta.Z = RiseSpeed * DeltaTime;
					this->FindComponentByClass<UAudioComponent>()->Play();
				}
			}
		AttachedMeshes[0]->AddRelativeLocation(Delta);
	}
	
}

// Called to bind functionality to input
void ASpirit_temp::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASpirit_temp::spiritMovement(int movement) {
	RootComponent->SetWorldLocation(pathPointLocation[movement]);//just move the player to the next sampled point on the spline
	FindComponentByClass<USkeletalMeshComponent>()->SetRelativeRotation(pathPointRotation[movement]);//and give the player the same rotation as the sampled point
}

