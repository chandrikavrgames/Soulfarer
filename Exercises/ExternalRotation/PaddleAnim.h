// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Exercises/ExternalRotation/Boat.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PaddleAnim.generated.h"

/**
 * 
 */
UCLASS()
class SKYFARER_C_API UPaddleAnim : public UAnimNotify
{
	GENERATED_BODY()

public:
	bool SpeedUp = false;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	ABoat* pBoat;
	TArray<AActor*> Boats;
};
