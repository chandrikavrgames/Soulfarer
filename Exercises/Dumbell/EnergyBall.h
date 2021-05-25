// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Inputs/Rig/GenericRigPawn.h"
#include "EnergyBall.generated.h"

UCLASS()
class SKYFARER_C_API AEnergyBall : public AGenericRigPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnergyBall();
	float lastX = 0;
	float lastY = 0;
	FVector startLoc;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void setMouseX(float x);
	void setMouseY(float y);
	void SetYValueRig();
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* SphereStaticMesh;
	UPROPERTY(EditAnywhere)
		float maxVelocity = 1;

private:
	float blendDuration = 0.5;
	float penaltyTime = 0;
	FVector previousRigLoc;
	UMaterialInstanceDynamic* DynamicMaterial;

};
