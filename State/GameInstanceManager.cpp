// Fill out your copyright notice in the Description page of Project Settings.
#include "GameInstanceManager.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Mouse_PlayerController.h"
#include "Exercises/ExternalRotation/Bucket.h"
#include "Exercises/ExternalRotation/Boat.h"
#include "UObject/ConstructorHelpers.h"

UGameInstanceManager::UGameInstanceManager(const FObjectInitializer& ObjectInitializer) : UGameInstance(ObjectInitializer)
{
	// This is for the eventual credits scene
	ConstructorHelpers::FClassFinder<UUserWidget> CreditsClassFinder(TEXT("/Game/Assets/UI/Credits"));
	CreditsClass = CreditsClassFinder.Class;
}

//bool UGameInstanceManager::bFirstStatePossessed = true;

void UGameInstanceManager::Init() {
	//pRigInputManager = NewObject<ARigInputController>(this);
	//UE_LOG(LogTemp, Warning, TEXT("RIG INPUT ALIVEish"));
	//if (pRigInputManager->IsConnected()) {
	//	pRigInputManager->UpdateSensorData();
	//}

}

void UGameInstanceManager::PossessFirstState() {
	if (bFirstStatePossessed) {
		//UE_LOG(LogTemp, Warning, TEXT("STARTING CALIBRATION..."));
		//if (pRigInputManager->)
		SwitchPossession(pRigInputManager, *StatePawnAssociation.Find(GameplayOrder[0]));
		bFirstStatePossessed = false;
		FVector InitialPosition = FVector(-5858.625977, 8080.887695, 335.850189);
		//pACamera->TeleportTo(InitialPosition, pACamera->GetActorRotation());
	}
}

void UGameInstanceManager::remoteTriggerSpiritAction(int state) {
	pASpirit->TriggerSpiritAction(state);
}
void UGameInstanceManager::remoteTriggerSpiritMovement(int movement) {
	pASpirit->spiritMovement(movement);
}

void UGameInstanceManager::SetGameplayOrder(TArray<EGameStates> NewGameplayOrder) {
	GameplayOrder.Empty();
	
	if (NewGameplayOrder[0] == EGameStates::Calibration) {
		GameplayOrder.Add(EGameStates::Calibration);
		NewGameplayOrder.Remove(EGameStates::Calibration);
	}

	GameplayOrder.Add(EGameStates::Spirit);

	for (int32 i = 0; i < NewGameplayOrder.Num(); i++) {
		GameplayOrder.Add(NewGameplayOrder[i]);
		UE_LOG(LogTemp, Warning, TEXT("%d"), (int)NewGameplayOrder[i]);
	}

	GameplayOrder.Add(EGameStates::Spirit);
	PossessFirstState();
}

void UGameInstanceManager::TransitionGameState() {
	CurrentGameState++;
	if (CurrentGameState < GameplayOrder.Num()) {

		EGameStates PreviousState = GameplayOrder[CurrentGameState-1];
		EGameStates CurrentState = GameplayOrder[CurrentGameState];

		SwitchPossession(*StatePawnAssociation.Find(PreviousState), *StatePawnAssociation.Find(CurrentState));

	}
}

void UGameInstanceManager::SwitchPossession(APawn* Previous, APawn* Next) {
	
	//(Cast<ABucket>(Next))->TeleportToStart();

	UE_LOG(LogTemp, Warning, TEXT("%s"), *Next->GetName());

	if (pAPaddleRotation->AutoPossessPlayer != EAutoReceiveInput::Player1 && Next != pASpirit) {
		pAPaddleRotation->AutoPossessPlayer = EAutoReceiveInput::Player1;
	}
	if (Next == pASpirit) {
		pAPaddleRotation->AutoPossessPlayer = EAutoReceiveInput::Disabled;
		Next->AutoPossessPlayer = EAutoReceiveInput::Player2;
	}
	else {
		
		Previous->AutoPossessPlayer = EAutoReceiveInput::Disabled;
		Previous->AutoReceiveInput = EAutoReceiveInput::Disabled;
		Next->AutoPossessPlayer = EAutoReceiveInput::Player0;
		Next->AutoReceiveInput = EAutoReceiveInput::Player0;

		if (Next->IsA(APawn::StaticClass())) {
			AMouse_PlayerController* PawnPlayerController = Cast<AMouse_PlayerController>(Next->GetWorld()->GetFirstPlayerController());
			if (PawnPlayerController) {
				PawnPlayerController->AutoReceiveInput = EAutoReceiveInput::Player0;
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("FAILED TO GET PLAYER CONTROLLER..."));
			}
		}
		AController* Controller = Previous->GetController();
		Controller->UnPossess();
		Controller->Possess(Next);
	}
	//FHitResult Graveyard;
	//Previous->K2_SetActorLocation({ -1000, -1000, -1000 }, false, Graveyard, true);
	
	

	//FVector CurrentCameraLocation = pACamera->K2_GetActorLocation();
	//CurrentCameraLocation.X += 840;
	//FHitResult CameraHitResult;
	//pACamera->K2_SetActorLocation(CurrentCameraLocation, true, CameraHitResult, false);
	
}

void UGameInstanceManager::OnStart() {
	//arrays to store all actors of the specified types
	TArray<AActor*> OutActorsCalibration;
	TArray<AActor*> OutActorsExternalRotation;
	TArray<AActor*> OutActorsBoats;
	TArray<AActor*> OutActorsRowing;
	TArray<AActor*> OutActorsPulldown;
	TArray<AActor*> OutActorsDumbbell;
	TArray<AActor*> OutActorsSpirits;
	TArray<AActor*> OutActorsRigInputController;
	TArray<AActor*> OutActorsCameras;

	//Storing all calibration actors
	UGameplayStatics::GetAllActorsOfClass
	(
		this,
		ACalibrationManager::StaticClass(),
		OutActorsCalibration
	);
	if (OutActorsCalibration.Num() > 0) {
		pCalibrationManager = Cast<ACalibrationManager>(OutActorsCalibration[0]);
	}
	//end calibration actors
	UGameplayStatics::GetAllActorsOfClass
	(
		this,
		ABucket::StaticClass(),
		OutActorsExternalRotation
	);
	if (OutActorsExternalRotation.Num() > 0) {
		pABucket = Cast<ABucket>(OutActorsExternalRotation[0]);
	}
	//storing all external rotation actors, in this case bucket pawns
	UGameplayStatics::GetAllActorsOfClass
	(
		this,
		ABoat::StaticClass(),
		OutActorsBoats
	);
	if (OutActorsBoats.Num() > 0) {
		pABoat = Cast<ABoat>(OutActorsBoats[0]);
	}
	//end external rotation actors

	//storing all rowing actors, in this case paddle rotation
	UGameplayStatics::GetAllActorsOfClass
	(
		this,
		APaddleRotation::StaticClass(),
		OutActorsRowing
	);
	if (OutActorsRowing.Num() > 0) {
		pAPaddleRotation = Cast<APaddleRotation>(OutActorsRowing[0]);
	}
	// end rowing actors

	// storing all pulldown actors, in this case movehand
	UGameplayStatics::GetAllActorsOfClass
	(
		this,
		AMoveHand::StaticClass(),
		OutActorsPulldown
	);
	if (OutActorsPulldown.Num() > 0) {
		pAMoveHand = Cast<AMoveHand>(OutActorsPulldown[0]);
	}

	//storing all dumbbell raise actors, in this case EnergyBall
	UGameplayStatics::GetAllActorsOfClass
	(
		this,
		AEnergyBall::StaticClass(),
		OutActorsDumbbell
	);
	if (OutActorsDumbbell.Num() > 0) {
		pAEnergyBall = Cast<AEnergyBall>(OutActorsDumbbell[0]);
	}
	//end dumbbell actors

	UGameplayStatics::GetAllActorsOfClass
	(
		this,
		ARigInputController::StaticClass(),
		OutActorsRigInputController
	);
	if (OutActorsRigInputController.Num() > 0) {
		UE_LOG(LogTemp, Warning, TEXT("GOT RIG"));
		pRigInputManager = Cast<ARigInputController>(OutActorsRigInputController[0]);
		//pRigInputManager->AddToRoot();
	}

	UGameplayStatics::GetAllActorsOfClass
	(
		this,
		ACameraActor::StaticClass(),
		OutActorsCameras
	);
	if (OutActorsCameras.Num() > 0) {
		pACamera = Cast<ACameraActor>(OutActorsCameras[0]);
		//pRigInputManager->AddToRoot();
	}

	UGameplayStatics::GetAllActorsOfClass
	(
		this,
		ASpirit_temp::StaticClass(),
		OutActorsSpirits
	);
	if (OutActorsSpirits.Num() > 0) {
		pASpirit = Cast<ASpirit_temp>(OutActorsSpirits[0]);
		//pRigInputManager->AddToRoot();
	}

	StatePawnAssociation = {
		TPair<EGameStates, AGenericRigPawn*>(EGameStates::Warmup, nullptr),
		TPair<EGameStates, AGenericRigPawn*>(EGameStates::Calibration, pCalibrationManager),
		TPair<EGameStates, AGenericRigPawn*>(EGameStates::ExternalRotation, pABoat),
		TPair<EGameStates, AGenericRigPawn*>(EGameStates::Rowing, pABoat),
		TPair<EGameStates, AGenericRigPawn*>(EGameStates::Pulldown, pAMoveHand),
		TPair<EGameStates, AGenericRigPawn*>(EGameStates::Dumbbell, pAEnergyBall),
		TPair<EGameStates, AGenericRigPawn*>(EGameStates::Spirit, pASpirit),
		TPair<EGameStates, AGenericRigPawn*>(EGameStates::Cooldown, nullptr)
	};
}

void UGameInstanceManager::Shutdown() {
	if (pRigInputManager) pRigInputManager->CloseSerialPort();
	UE_LOG(LogTemp, Warning, TEXT("RIG INPUT DEADish"));
	bFirstStatePossessed = true;
}

void UGameInstanceManager::EnableCredits() {
	pCredits = CreateWidget<UCreditsUserWidget>(this, CreditsClass);
	pCredits->Setup();
}
UGameInstanceManager::~UGameInstanceManager()
{
	// delete pRigInputManager;
}
