// Fill out your copyright notice in the Description page of Project Settings.

#include "Mouse_GameModeBase.h"
#include "Mouse_PlayerController.h"

AMouse_GameModeBase::AMouse_GameModeBase() {
	PlayerControllerClass = AMouse_PlayerController::StaticClass();
}


