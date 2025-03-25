// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPossessDelegate);

/**
 * 
 */
UCLASS()
class TETRIS_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void OnPossess(APawn* NewPawn) override;

public:
	UFUNCTION(Exec)
	void Test();

public:
	UPROPERTY(BlueprintAssignable)
	FOnPossessDelegate OnPossessDelegate;
};
