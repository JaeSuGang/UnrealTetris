// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Block/Block.h"
#include "MainCharacter.generated.h"

#define BoardWidthPadding 200.0f
#define BoardHeightPadding 200.0f

struct FInputActionValue;


class UCameraComponent;
class ABlock;
class UInputAction;
class UInputMappingContext;

UCLASS()
class TETRIS_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* NewInputComponent) override;

public:
	UFUNCTION(BlueprintCallable)
	FBlockTypeInfoRow& FindBlockInfoRowByEnum(EBlockShapeAndColor FindingShape);

	UFUNCTION(BlueprintCallable)
	void SpawnRandomNewBlock();

	UFUNCTION(BlueprintCallable)
	void CheckBlockComplete();

	UFUNCTION(BlueprintCallable)
	void SetCameraToCenter();

	UFUNCTION(BlueprintCallable)
	void ResetFocusLocation();

	UFUNCTION(BlueprintCallable)
	void SetBoardSize(int nWidth, int nHeight);

	UFUNCTION(BlueprintCallable)
	void DestroyControllingBlocks();

	UFUNCTION(BlueprintCallable)
	bool SpawnControllingBlocks(EBlockShapeAndColor Color, FVector PivotPosition, int NewDirection = 0);

	UFUNCTION(BlueprintCallable)
	void RotateControllingBlock();

	UFUNCTION(BlueprintCallable)
	void MoveControllingBlockLeft();

	UFUNCTION(BlueprintCallable)
	void MoveControllingBlockRight();

	UFUNCTION(BlueprintCallable)
	void MoveControllingBlockDown();

	UFUNCTION(BlueprintCallable)
	void MoveControllingBlockUp();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tetris)
	UInputMappingContext* DefaultIMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tetris)
	UInputAction* BlockLeftIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tetris)
	UInputAction* BlockRightIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tetris)
	UInputAction* BlockDownIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tetris)
	UInputAction* BlockUpIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tetris)
	UInputAction* BlockRotateIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tetris)
	TSubclassOf<ABlock> BlockClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tetris, meta = (AllowedStructTypes = "FBlockTypeInfoRow"))
	UDataTable* BlockTypeInfoTable;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tetris)
	USceneComponent* DefaultSceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tetris)
	UCameraComponent* CameraComponent;

protected:
	UPROPERTY(BlueprintReadWrite)
	FVector BlockFocusLocation;

	UPROPERTY(BlueprintReadWrite)
	AActor* CameraActor;

	UPROPERTY(BlueprintReadWrite)
	TArray<ABlock*> ControllingBlocks;

	UPROPERTY(BlueprintReadWrite)
	TArray<ABlock*> BoundaryBlocks;

	UPROPERTY(BlueprintReadWrite)
	EBlockShapeAndColor ControllingBlockShapeAndColor;

	UPROPERTY(BlueprintReadWrite)
	int ControllingBlockDirection;

	UPROPERTY()
	int BoardWidth;

	UPROPERTY()
	int BoardHeight;
};
