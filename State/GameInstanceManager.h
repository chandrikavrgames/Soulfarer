// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GenericInclude.h"
#include "Engine/GameInstance.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "UI/CreditsUserWidget.h"
#include "GameInstanceManager.generated.h"

/**
 * 
 */
UCLASS()
class SKYFARER_C_API UGameInstanceManager : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UGameInstanceManager(const FObjectInitializer& ObjectInitializer);
	~UGameInstanceManager();
	virtual void OnStart() override;
	virtual void Shutdown() override;
	virtual void Init() override;

	ACalibrationManager* pCalibrationManager;
	AEnergyBall* pAEnergyBall;
	AMoveHand* pAMoveHand;
	APaddleRotation* pAPaddleRotation;
	ABoat* pABoat;
	ABucket* pABucket;
	ASpirit_temp* pASpirit;
	ACameraActor* pACamera;
	UCreditsUserWidget* pCredits;

	void EnableCredits();
	void remoteTriggerSpiritAction(int state);
	void remoteTriggerSpiritMovement(int movement);
	void PossessFirstState();
	void TransitionGameState();
	void SwitchPossession(APawn* Previous, APawn* Next);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Order")
	void SetGameplayOrder(TArray<EGameStates> NewGameplayOrder);

	ARigInputController* pRigInputManager;

private:
	bool bFirstStatePossessed = true;
	int32 CurrentGameState = 0;

	TArray<EGameStates> GameplayOrder =
	{
		//EGameStates::Calibration,
		EGameStates::ExternalRotation,
		EGameStates::Rowing,
		EGameStates::Pulldown,
		EGameStates::Dumbbell
	};

	TMap<EGameStates, AGenericRigPawn*> StatePawnAssociation;

	UPROPERTY(Category = Credits, VisibleAnywhere)
	TSubclassOf<class UUserWidget> CreditsClass;

	//TMap<UCustomDataStructures::Exercises, AActor*> ExerciseClassTypes;
};
