// Fill out your copyright notice in the Description page of Project Settings.

#include "Bucket.h"
#include "Components/InputComponent.h"
#include "Components/AudioComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Sound/SoundCue.h"
//#include "Particles/ParticleSystemComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ABucket::ABucket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//reset velocity list
	for (int i = 0; i < maxVelList; i++) {
		arr_velocities.Add(0);
	}


	ExerciseSensors = 
	{
		UCustomDataStructures::MidLeft,
		UCustomDataStructures::MidRight
	};

	BoatDelta = { 0, 0, 30 };
	//instatiate audio
	/*static ConstructorHelpers::FObjectFinder<USoundCue> DingSoundCueObject(TEXT("SoundCue'/Game/FirstPerson/Audio/ding_cue.ding_cue'"));
	if (DingSoundCueObject.Succeeded())
	{
		ding_cue = DingSoundCueObject.Object;

		DingAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("DingAudioComponent"));
		DingAudioComponent->SetupAttachment(RootComponent);

	}
	static ConstructorHelpers::FObjectFinder<USoundCue> BuzzSoundCueObject(TEXT("SoundCue'/Game/FirstPerson/Audio/buzz_cue.buzz_cue'"));
	if (BuzzSoundCueObject.Succeeded())
	{
		buzz_cue = BuzzSoundCueObject.Object;

		BuzzAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BuzzAudioComponent"));
		BuzzAudioComponent->SetupAttachment(RootComponent);

	}*/

}

// Called when the game starts or when spawned
void ABucket::BeginPlay()
{
	Super::BeginPlay();
	this->GetComponents<UStaticMeshComponent>(CoconutMesh);
	StartLocation = CoconutMesh[0]->K2_GetComponentLocation();

	PreviousRigLocation = { 0,0,0 };

	//changing color
	/*objectMesh = FindComponentByClass<UStaticMeshComponent>();
	auto material = objectMesh->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(material, NULL);
	objectMesh->SetMaterial(0, DynamicMaterial);
	//set sound cues to audio Components*/
	/*if (DingAudioComponent && ding_cue)
	{
		DingAudioComponent->SetSound(ding_cue);
	}
	if (BuzzAudioComponent && buzz_cue)
	{
		BuzzAudioComponent->SetSound(buzz_cue);
	}*/

	//create particle system
	/*successEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles"));

	successEffect->AttachTo(objectMesh);
	successEffect->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> successEffectObject(TEXT("ParticleSystem'/Game/Assets/Materials/Particles/test_particle.test_particle'"));
	if (successEffectObject.Succeeded())
	{
		successEffect->SetTemplate(successEffectObject.Object);
	}*/
}

void ABucket::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MouseX", this, &ABucket::externalRotation);
}
// Called every frame
void ABucket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//successEffect->ActivateSystem();
	if (this->AutoPossessPlayer == EAutoReceiveInput::Player0) {
		SerialData = UpdateSerialData(ExerciseSensors[bUsingLeft]);

		if (SerialData.Velocities.Z > GetCalibration(ExerciseSensors[0]).ZVelocityThreshold[1]) {
			//DingAudioComponent->Play(0.5f);
		}

		// Color it red.  Read the instructions in updateLocation() for more info

		//check if there is a penalty
		/*if (penaltyTimeLeft > 0) {
			//if there is, get the ratio
			float penaltyColorBlend = penaltyTimeLeft / speedingPenaltyTime;//.05f + FMath::Cos(penaltyTimeLeft)/2;
			//sets the interpolation value for default -> penalty color
			DynamicMaterial->SetScalarParameterValue(TEXT("PenaltySpeedBlend"), penaltyColorBlend);
			penaltyTimeLeft -= DeltaTime;

		}
		else {
			//otherwise, no interpolation
			float penaltyColorBlend = 0;//.05f + FMath::Cos(penaltyTimeLeft)/2;
			DynamicMaterial->SetScalarParameterValue(TEXT("PenaltySpeedBlend"), penaltyColorBlend);

		}*/

	}

	updateLocation();
	
	 
}


void ABucket::TeleportToStart() {
	if (abs(CoconutMesh[0]->K2_GetComponentLocation().Z - StartLocation.Z) > 10) {
		FHitResult CoconutTeleportHitResult;
		CoconutMesh[0]->K2_SetWorldLocation(StartLocation, false, CoconutTeleportHitResult, true);
		CheckForExerciseReset = true;
		CurrentDirection = -1;
	}
}


void ABucket::updateLocation() {
	
	if (UsingRig) CalcDeltaLoc(ExerciseSensors[0]);

	/*if (RepUnsync) {
		DeltaLocation = FVector::ZeroVector;
	}
	else*/


	//put color changing before the DeltaLocation math
	/*

	Check MoveHand.cpp for more stuff about this. Also check if you want to use averages instead of the immediate deltaLocation
	
	Color changing:
	0. Set material of mesh to ColorChanging. Should be in the materials folder. If not, take the test material for this (check the original test scenes if you can't find it) and MAKE A COPY. Rename the LERP variables to be something clearer
	1. Calculate the velocity or delta location. DeltaLocation.size?
	2. Check whether the average velocity is greater than the maximum velocity. 
		a. If it is, set penaltyTimeLeft = speedPenaltyTime
			i. In Tick, if penaltyTimeLeft > 0, penaltyTimeLeft -= DeltaTime
		b. If it is not, proceed
	3. float representing PenaltySpeedBlend = normalize penaltyTimeLeft (so like penaltyTimeLeft/speedPenaltyTime). Set that variable to the LERP variable in the material
		ex: float penaltyColorBlend = penaltyTimeLeft/speedPenaltyTime
		DynamicMaterial->SetScalarParameterValue(TEXT("PenaltySpeedBlend"), penaltyColorBlend);
	4. Check how close the average velocity is to the maximum velocity
	5. float representing OkaySpeedBlend = ave/max. Set that variable to the LERP variable in the material
		ex: float speedColorBlend = averageVelocity/maxVelocity
		DynamicMaterial->SetScalarParameterValue(TEXT("OkaySpeedBlend"), speedColorBlend);

	Speed penalty:
	Possible penalty: if penaltyTimeLeft > 0, deltaLocation * speedPenalty. That way, you probably won't make it to the threshold by the end of your stroke and will reset
	Or whatever penalty you want for going too fast


	Encouraging people to do the full stroke:
	For the first ~75% of the pull, don't do anything with the specularity. For the last 25%, set it to 1
	To determine what the first and last parts of a pull is, check the calibrations (or hard code some for testing)
	Make sure to check what direction you are going in, cause the last 25% of a pull is in a different location depending on what direction it's in




	Holding (might be removed later):
	if in the holding phase, maybe don't do any of the color changing steps in favor of some way to show that you should hold for X many frames. 
	REMEMBER TO CHANGE THAT HARD CODED 120 IN GENERICRIGPAWN INTO A VARIABLE THAT WE CAN ACCESS
	


	
	*/

	

	//check if magnitude is greater than the maximum velocity
	if (abs(DeltaLocation.Z) > maxDeltaLocationSize) {
		//UE_LOG(LogTemp, Display, TEXT("Too fast"));
		//if it is, set the penalty time
		penaltyTimeLeft = speedingPenaltyTime;
		//reset the list
		zeroVelocityList();
		
		//if (DeltaLocation.Size() > 0.0f) UE_LOG(LogTemp, Display, TEXT("size: %f"), DeltaLocation.Size());
		//penalty
		/*DeltaLocation.X *= speedPenalty;
		DeltaLocation.Y *= speedPenalty;
		DeltaLocation.Z *= speedPenalty;*/
		//play buzz
		/*if (BuzzAudioComponent && buzz_cue && !BuzzAudioComponent->IsPlaying())
		{
			BuzzAudioComponent->Play(0.1f);
			
		}*/
	}
	else {
		//add deltaLocation to list
		replaceVelocityInArray(DeltaLocation.Z);
		//otherwise, interpolate between the colors between the max and the current velocities
		//using average to prevent flashing
		float blendOk = getAverageVelocity() / maxDeltaLocationSize;// / maxVelocity.Z;
		//UE_LOG(LogTemp, Display, TEXT("average: %f"), blendOk);
		//float blendOk = abs(hand_velocity.Z) / maxVelocity.Z;
		//DynamicMaterial->SetScalarParameterValue(TEXT("OkaySpeedBlend"), blendOk);
		//UE_LOG(LogTemp, Display, TEXT("OKAY average: %f"), getAverageVelocity());

	}

	//where the bucket will move
	
	float PositionDifference = abs(GetPositionDifference(ExerciseSensors[0]));

	// X DISTANCE IS 72

	float RatioRigToBucketX = PositionDifference / 36;
	float RatioRigToBucketZ = SlopeMagnitude * PositionDifference / abs(-0.1*(GetPositionDifference(ExerciseSensors[0])));

	//to get the right movements, there is randomass math involved. Y'all explain what the math is in the comments 
	DeltaLocation.X = abs(DeltaLocation.Z) * RatioRigToBucketX;//TotalReps);
	DeltaLocation.Y = 0;
	DeltaLocation.Z = CoconutDirection * abs(DeltaLocation.Z) * RatioRigToBucketZ;
	

	// First check if coconut has reached the midway point of its motion and therefore the player is about to start retracting
	//55 comes from the fact that the bucket is -45 below the z plane.

	if (CoconutDirection == -1) {

		/*if ((RepUnsync && FramesUnsync > 30) || FramesFrozen > 180) {
			TeleportToStart();
		}*/

		// Explanation for the math
		// You take the calibrated distance which has a numerical value in the hundreds. 
		// 
		//UE_LOG(LogTemp, Warning, TEXT("%f : %f"), CoconutMesh[0]->K2_GetComponentLocation().Z, (-0.2*(55 + abs(GetPositionDifference(ExerciseSensors[0]))) + 30 * (3 - TotalReps)));
		if (CoconutMesh[0]->K2_GetComponentLocation().Z < (BoatDelta.Z * (3 - TotalReps) + StartLocation.Z + -0.2*(abs(GetPositionDifference(ExerciseSensors[0]))))) {

			//ResetSyncVars();
			CoconutDirection = 1;

		}



	}

	if (CoconutDirection == 1) {

		if ((RepUnsync && FramesUnsync > 20) || FramesFrozen > 180) {
			TeleportToStart();
		}

		if (CoconutMesh[0]->K2_GetComponentLocation().Z > StartLocation.Z + 5)
		{
			//ResetSyncVars();
			FHitResult BoatElevationHitResult;
			StartLocation += BoatDelta;
			TeleportToStart();
			//GetAttachParentActor()->K2_SetActorLocation();
			GetAttachParentActor()->K2_AddActorWorldOffset(BoatDelta, false, BoatElevationHitResult, true);
			CoconutDirection = -1;
			//DingAudioComponent->Play(0.5f);

			//TotalReps--;
			if (TotalReps == 0) {
				RollCredits();
				//TransitionGameState();
			}
		}
	}
	//where the coconut gets moved
	CoconutMesh[0]->AddRelativeLocation(DeltaLocation);
}




void ABucket::externalRotation(float value) {
	DeltaLocation = FVector(0, 0, value);
}

void ABucket::replaceVelocityInArray(float newVelocity) {
	arr_velocities.RemoveAt(0, 1, true); //Pop();
	arr_velocities.Insert(newVelocity, maxVelList - 1);//Push(newVelocity);
}

float ABucket::getAverageVelocity() {
	float average = 0;
	for (int i = 0; i < maxVelList; i++) {
		average += abs(arr_velocities[i]);
	}
	average /= maxVelList;
	//UE_LOG(LogTemp, Display, TEXT("average: %f"), average);
	return average;
}

void ABucket::zeroVelocityList() {
	for (int i = 0; i < maxVelList; i++) {
		arr_velocities[i] = 0;
	}
}