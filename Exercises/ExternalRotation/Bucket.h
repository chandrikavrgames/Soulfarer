// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inputs/Rig/GenericRigPawn.h"
#include "Bucket.generated.h"

UCLASS()
class SKYFARER_C_API ABucket : public AGenericRigPawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABucket();

	//UPROPERTY(EditAnywhere, Category = Bucket)
	//	//how long you are penalized for in seconds
	//	float speedingPenaltyTime = .5;
	//values for speed penalty
	UPROPERTY(EditAnywhere, Category = Bucket)
		//how long you are penalized for in seconds
		float speedingPenaltyTime = .5f;
	UPROPERTY(EditAnywhere, Category = Bucket)
		//how much slower you move
		float  speedPenalty = .01;
	//maximum deltaLocation before you get the penalty
	UPROPERTY(EditAnywhere, Category = Bucket)
		float maxDeltaLocationSize = 3;
	//audio cues
	UPROPERTY(EditAnywhere, Category = Audio)
		class USoundCue* ding_cue;
	UPROPERTY(EditAnywhere, Category = Audio)
		class USoundCue* buzz_cue;
	UPROPERTY(EditAnywhere, Category = Particle)
		class UParticleSystemComponent* successEffect;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<UStaticMeshComponent*> CoconutMesh;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void externalRotation(float value);

	void TeleportToStart();
	 
private:
	
	UPROPERTY(EditAnywhere)
		int CoconutDirection = -1;

	UPROPERTY(EditAnywhere)
		float SlopeMagnitude = 4;

	FVector StartLocation;
	FVector BoatDelta;

	//object movement
	void updateLocation();

	//playing audio
	//UAudioComponent* DingAudioComponent;
	//UAudioComponent* BuzzAudioComponent;


	//changing colors
	UMaterialInstanceDynamic* DynamicMaterial;
	float penaltyTimeLeft = 0;
	UStaticMeshComponent* objectMesh;

	int maxVelList = 100;
	TArray<float> arr_velocities;
	void replaceVelocityInArray(float newVelocity);
	float getAverageVelocity();
	void zeroVelocityList();
	
	
};
