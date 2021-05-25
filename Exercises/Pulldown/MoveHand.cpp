// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveHand.h"

#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
//#include "Runtime/Engine/Classes/Engine/Engine.h"
// Sets default values
AMoveHand::AMoveHand()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	for (int i = 0; i < maxVelList; i++) {
		arr_velocities.Add(FVector(0, 0, 0));
	}

	ExerciseSensors =
	{
		UCustomDataStructures::UpperLeft,
		UCustomDataStructures::UpperRight
	};

	if (UsingRig) {
		maxVelocity_left = FVector(0, 0, 1);//FVector(0, 0, GetCalibration(ExerciseSensors[0]).ZVelocityThreshold[1]);
		startPullPosition_left = FVector(0, 0, 800); //FVector(0, 0, GetCalibration(ExerciseSensors[0]).StartPositionThreshold[0]);
		endPullPosition_left = FVector(0, 0, 600);//FVector(0, 0, GetCalibration(ExerciseSensors[0]).EndPositionThreshold[1]);

		maxVelocity_right = maxVelocity_left; //FVector(0, 0, GetCalibration(ExerciseSensors[1]).ZVelocityThreshold[1]);
		startPullPosition_right = startPullPosition_left; //FVector(0, 0, GetCalibration(ExerciseSensors[1]).StartPositionThreshold[0]);
		endPullPosition_right = endPullPosition_left; // FVector(0, 0, GetCalibration(ExerciseSensors[1]).EndPositionThreshold[1]);


		if (usingLeft) {
			startPullPosition = startPullPosition_left;
			endPullPosition = endPullPosition_left;
			maxVelocity = maxVelocity_left;
		}
		else {
			startPullPosition = startPullPosition_right;
			endPullPosition = endPullPosition_right;
			maxVelocity = maxVelocity_right;
		}


	}

	


}

// Called when the game starts or when spawned
void AMoveHand::BeginPlay()
{
	Super::BeginPlay();

	//set max height on screen
	
	FVector pos = GetActorLocation();
	//FVector pos = GetAttachParentActor()->GetActorLocation();

	maxHeight = pos + relativeMaxHeight;
	cur_handPosition = pos;

	//changing color 
	ball = FindComponentByClass<UStaticMeshComponent>();
	auto material = ball->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(material, NULL);
	ball->SetMaterial(0, DynamicMaterial);

	//rig stuff
	//UGameInstance* UGame = GetGameInstance();
	//UGameInstanceManager* UGameManager = static_cast<UGameInstanceManager*>(UGame);
	//pRigInputManager = UGameManager->pRigInputManager;
	

}

// Called every frame
void AMoveHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (this->AutoPossessPlayer == EAutoReceiveInput::Player0) {

		if (!ball->IsVisible()) {
			ball->SetVisibility(true, true);
		}
		
		if (UsingRig) {
			SetZVelocityRig();
		}
		
		//color changing stuff: https://www.youtube.com/watch?v=AsAx9HObtIY
		if (penaltyTimeLeft > 0) {
			penaltyTimeLeft -= DeltaTime;
			float blend = penaltyTimeLeft/speedingPenaltyTime;//.05f + FMath::Cos(penaltyTimeLeft)/2;
			//sets the interpolation value for default -> penalty color
			DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), blend);
			
		}
		else {
			float blend = 0;//.05f + FMath::Cos(penaltyTimeLeft)/2;
			DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), blend);
			
		}

		SetActorLocation(cur_handPosition);
		//GetAttachParentActor()->SetActorLocation(cur_handPosition);

		

	}
	
	
	


}

// Called to bind functionality to input
void AMoveHand::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//UE_LOG(LogTemp, Display, TEXT("Calling Ball InputComponent"));
	if (!UsingRig) {
		Super::SetupPlayerInputComponent(PlayerInputComponent);
		//get mouse inputs. From Project Settings
		check(PlayerInputComponent);
		//PlayerInputComponent->BindAxis("MouseX", this, &AMoveHand::setXVelocity);
		PlayerInputComponent->BindAxis("MouseY", this, &AMoveHand::setYVelocity);
	}
	
	
}
void AMoveHand::setXVelocity(float x) {
	if (Controller != NULL && x != 0) {
		//UE_LOG(LogTemp, Display, TEXT("X: %f"), x);
	}
	//hand_velocity.Y = x;

	
}

void AMoveHand::setYVelocity(float y) {
	//if (Controller != NULL && y != 0) {
		//UE_LOG(LogTemp, Display, TEXT("Y: %f"), y);
	//}
	hand_velocity.Z = y;
	updateHandLocationVector();

	//if (GetWorld()) {
	//	FVector2D mouse_coordinates;
	//	GEngine->GameViewport->GetMousePosition(mouse_coordinates)
	//}

	//GetCursorPos(curMousePos);
	//curMousePos->x;
	
	
	
	

}

void AMoveHand::SetZVelocityRig() {
	if (usingLeft) {
		SerialData = UpdateSerialData(UCustomDataStructures::UpperLeft);
	}
	else {
		SerialData = UpdateSerialData(UCustomDataStructures::UpperRight);
	}
	
	hand_velocity.Z = -SerialData.Velocities.Z;
	//if (abs(SerialData.Velocities.Z) > 0.0) {
	//	UE_LOG(LogTemp, Display, TEXT("Z: %f, %f"), hand_velocity.Z, abs(SerialData.Velocities.Z));
	//}
	updateHandLocationVector();
	
}




void AMoveHand::updateHandLocationVector() {
	
	//if you go too fast in either direction, there is a penalty
	//if (hand_velocity.DotProduct(hand_velocity, hand_velocity) > maxVelocity.DotProduct(maxVelocity, maxVelocity)) {
	if (abs(hand_velocity.Z) > maxVelocity.Z){
		penaltyTimeLeft = speedingPenaltyTime;
		zeroVelocityList();

	}
	else {
		//for getting the average, for smooth color transition from default -> Good color
		replaceVelocityInArray(hand_velocity);
		//theoretically should be 0-1, but i don't know if it's actually doing that
		float blendOk = getAverageVelocity() / maxVelocity.Z;// / maxVelocity.Z;
		//UE_LOG(LogTemp, Display, TEXT("average: %f"), blendOk);
		//float blendOk = abs(hand_velocity.Z) / maxVelocity.Z;
		DynamicMaterial->SetScalarParameterValue(TEXT("BlendOkay"), blendOk);
	}

	//if the average of the last set of velocities is a different sign than the new average velocity
	//add average velocity and divide by max velocity like we did for BlendOkay, even if the average was zeroed, cause that's a penalty
	if (arr_previousAverageVelocities.Num() != 0) {

	}


	float speed;
	if (UsingRig) {
		zPos = SerialData.Positions.Z;
	}
	
	//see what direction it is going in. Moves slower if your arm is moving up
	if (hand_velocity.Z > 0) {
		
		speed = 1;


		//slightly slower if you're in the bottom 75% of the pull
		if (UsingRig) {
			
			/*
			------------ 800

			---------------- 600 + .75*(800 - 600)




			------------- 600
			*/
			if (SerialData.Positions.Z < endPullPosition.Z + 0.75*(startPullPosition.Z - endPullPosition.Z)) {
				speed = 1;
				

				specularness = 0;
			}
			else {
				speed = 1.75;
				specularness = 1;
			}
		}
		

	}
	else {

		speed = 3;
		
		//slightly slower if you're in the top 75% of the pull
		if (UsingRig) {
			/*
			------------ 800

			


			---------------- 800 - .75*(800 - 600)

			------------- 600
			*/


			if (SerialData.Positions.Z > startPullPosition.Z - 0.75*(startPullPosition.Z - endPullPosition.Z)) {
				speed = 2;
				specularness = 0;
				
			}
			else {

				speed = 3;
				specularness = 1;
			}
		}
		
		//UE_LOG(LogTemp, Display, TEXT("z: %f, speed: %f"), hand_velocity.Z, hand_velocity.DotProduct(hand_velocity, hand_velocity));

	}
	//else if (hand_velocity.Z > 0) {
	speed *= abs(hand_velocity.Z); //hand_velocity.DotProduct(hand_velocity, hand_velocity);
	FVector movingBy = angle * speed;
	if (penaltyTimeLeft > 0) {
		movingBy *= speedPenalty;
	}

	DynamicMaterial->SetScalarParameterValue(TEXT("BlendPosition"), specularness);





	cur_handPosition = GetActorLocation() + movingBy;
	//cur_handPosition = GetAttachParentActor()->GetActorLocation() + movingBy;


	

	if (cur_handPosition.Z >= maxHeight.Z) {
		//cur_handPosition.Z = maxHeight.Z;
		//cur_handPosition.Z = 0;
		FHitResult BoatWaterfallClimb;
		FVector BoatWorldLocation = GetAttachParentActor()->K2_GetActorLocation();
		BoatWorldLocation.Z += 7050/totalNumReps;
		GetAttachParentActor()->K2_SetActorLocation(BoatWorldLocation, false, BoatWaterfallClimb, true);
		//K2_AddActorWorldOffset({ 0,0,95 }, false, BoatWaterfallClimb, true);
		cur_handPosition.Z = (BoatWorldLocation.Z + 95);
		//cur_handPosition.Z += 95;
		maxHeight.Z = cur_handPosition.Z + relativeMaxHeight.Z;
		//usingLeft = !usingLeft;
		switchHands();
		//isActive = false;


	}


}


void AMoveHand::replaceVelocityInArray(FVector newVelocity) {
	arr_velocities.RemoveAt(0,1,true); //Pop();
	arr_velocities.Insert(newVelocity, maxVelList-1);//Push(newVelocity);
}

float AMoveHand::getAverageVelocity() {
	float average = 0;
	for (int i = 0; i < maxVelList; i++) {
		average += abs(arr_velocities[i].Z);
	}
	average /= maxVelList;
	//UE_LOG(LogTemp, Display, TEXT("average: %f"), average);
	return average;
}

void AMoveHand::zeroVelocityList() {
	for (int i = 0; i < maxVelList; i++) {
		arr_velocities[i] = (FVector(0, 0, 0));
	}
}

void AMoveHand::switchHands() {
	usingLeft = !usingLeft;
	if (usingLeft) {
		startPullPosition = startPullPosition_left;
		endPullPosition = endPullPosition_left;
		maxVelocity = maxVelocity_left;
	}
	else {
		startPullPosition = startPullPosition_right;
		endPullPosition = endPullPosition_right;
		maxVelocity = maxVelocity_right;
	}

	curRepsDone += 1;

	if (curRepsDone >= totalNumReps) {
		FHitResult BoatWaterfallClimb;
		//FVector BoatLocation = GetAttachParentActor()->K2_GetActorLocation();
		FVector BoatLocation = { -1560.000000, -9150.000000, 7050.000000 };
		GetAttachParentActor()->K2_SetActorLocation(BoatLocation, false, BoatWaterfallClimb, true);
		if (UsingRig) {
			TransitionGameState();
		}
		else {
			UE_LOG(LogTemp, Display, TEXT("Reps done. Did %i out of %i"), curRepsDone, totalNumReps);
		}
		
	}
	//maxVelocity = FVector(0, 0, GetCalibration(ExerciseSensors[0]).ZVelocityThreshold[1]);
	//minPullPosition = FVector(0, 0, GetCalibration(ExerciseSensors[0]).StartPositionThreshold[0]);
	//maxPullPosition = FVector(0, 0, GetCalibration(ExerciseSensors[0]).EndPositionThreshold[1]);

	//maxVelocity_right = FVector(0, 0, GetCalibration(ExerciseSensors[1]).ZVelocityThreshold[1]);
	//minPullPosition_right = FVector(0, 0, GetCalibration(ExerciseSensors[1]).StartPositionThreshold[0]);
	//maxPullPosition_right = FVector(0, 0, GetCalibration(ExerciseSensors[1]).EndPositionThreshold[1]);
}