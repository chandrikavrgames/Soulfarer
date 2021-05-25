// Fill out your copyright notice in the Description page of Project Settings.

#include "CreditsUserWidget.h"
#include "UObject/ConstructorHelpers.h"

UCreditsUserWidget::UCreditsUserWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)  {
}

void UCreditsUserWidget::Setup() {
		this->AddToViewport();

		UWorld* World = GetWorld();

		APlayerController* PlayerController = World->GetFirstPlayerController();

		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(this->TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PlayerController->SetInputMode(InputModeData);

		PlayerController->bShowMouseCursor = true;
}


