// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreditsUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SKYFARER_C_API UCreditsUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
		UCreditsUserWidget(const FObjectInitializer& ObjectInitializer);
		void Setup();
};
