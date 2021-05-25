// Fill out your copyright notice in the Description page of Project Settings.

#include "Mouse_PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"

AMouse_PlayerController::AMouse_PlayerController() {
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	EAutoReceiveInput::Player0;
}


