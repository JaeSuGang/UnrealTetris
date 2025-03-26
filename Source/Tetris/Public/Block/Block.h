// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

#define BlockLength 40.0f

UENUM(BlueprintType)
enum class EBlockShapeAndColor : uint8
{
	White,
	Red,
	Orange,
	Yellow,
	Green,
	SkyBlue,
	Blue,
	Purple,
	Black
};

USTRUCT(BlueprintType)
struct FBlockTypeInfoRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBlockShapeAndColor BlockShapeAndColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> DirectionA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> DirectionB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> DirectionC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> DirectionD;
};

UCLASS()
class TETRIS_API ABlock : public AActor
{
	GENERATED_BODY()
	
public:	
	ABlock();

protected:
	virtual void BeginPlay() override;

public:
	void SetColor(UMaterialInterface* NewMaterial);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* DefaultSceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowedStructTypes = "FBlockTypeInfoRow"))
	UDataTable* BlockTypeInfoTable;
};
