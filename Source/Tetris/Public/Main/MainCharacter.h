// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class UCameraComponent;

UCLASS()
class TETRIS_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

public:
	UFUNCTION(BlueprintCallable)
	void ViewAsCameraActor();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* CameraActor;
};
