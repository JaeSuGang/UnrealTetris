// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/MainPlayerController.h"

void AMainPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);

	OnPossessDelegate.Broadcast();
}

void AMainPlayerController::Test()
{
	OnPossessDelegate.Broadcast();
}
