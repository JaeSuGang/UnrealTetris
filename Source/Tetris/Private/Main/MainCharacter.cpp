

#include "Main/MainCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraActor.h"
#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/OverlapResult.h"

#include "Main/MainPlayerController.h"

AMainCharacter::AMainCharacter()
{
	GetCharacterMovement()->GravityScale = 0.0f;

	GetCapsuleComponent()->SetCapsuleSize(10.0f, 10.0f);

	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	RootComponent = DefaultSceneComponent;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);
	CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
	CameraComponent->bAutoCalculateOrthoPlanes = false;
	CameraComponent->bConstrainAspectRatio = true;
	CameraComponent->OrthoNearClipPlane = -1000.0f;
	CameraComponent->OrthoFarClipPlane = 1000.0f;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* NewInputComponent)
{	
	Super::SetupPlayerInputComponent(NewInputComponent);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = PC->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			EnhancedInputSubsystem->AddMappingContext(DefaultIMC, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(NewInputComponent))
	{
		EnhancedInput->BindAction(BlockLeftIA, ETriggerEvent::Started, this, &AMainCharacter::MoveControllingBlockLeft);
		EnhancedInput->BindAction(BlockRightIA, ETriggerEvent::Started, this, &AMainCharacter::MoveControllingBlockRight);
		EnhancedInput->BindAction(BlockDownIA, ETriggerEvent::Triggered, this, &AMainCharacter::MoveControllingBlockDown);
		EnhancedInput->BindAction(BlockUpIA, ETriggerEvent::Started, this, &AMainCharacter::MoveControllingBlockUp);
		EnhancedInput->BindAction(BlockRotateIA, ETriggerEvent::Started, this, &AMainCharacter::RotateControllingBlock);
	}
}

FBlockTypeInfoRow& AMainCharacter::FindBlockInfoRowByEnum(EBlockShapeAndColor FindingShape)
{
	FBlockTypeInfoRow* FoundRow{};
	TArray<FBlockTypeInfoRow*> Rows;
	BlockTypeInfoTable->GetAllRows<FBlockTypeInfoRow>(TEXT("RowFetch"), Rows);

	for (FBlockTypeInfoRow* LoopRow : Rows)
	{
		if (LoopRow->BlockShapeAndColor == FindingShape)
		{
			FoundRow = LoopRow;
			break;
		}
	}

	return *FoundRow;
}

void AMainCharacter::SpawnRandomNewBlock()
{
	ResetFocusLocation();

	EBlockShapeAndColor NewShape = (EBlockShapeAndColor)FMath::RandRange((int)EBlockShapeAndColor::Red, (int)EBlockShapeAndColor::Purple);
	SpawnControllingBlocks(NewShape, BlockFocusLocation);
}

void AMainCharacter::CheckBlockComplete()
{
	FCollisionShape OverlapBox = FCollisionShape::MakeBox(FVector(1.0f, (BoardWidth - 1) * BlockLength, 1.0f));

	for (int i = 0; i < BoardHeight; ++i)
	{
		TArray<FOverlapResult> OverlapResults{};
		GetWorld()->OverlapMultiByObjectType(OverlapResults, GetActorLocation() + FVector{ 0.0f, (BoardWidth - 1) * BlockLength / 2.0f, i * BlockLength }, FQuat::Identity, FCollisionObjectQueryParams::AllDynamicObjects, OverlapBox, FCollisionQueryParams::DefaultQueryParam);
		if (OverlapResults.Num() >= BoardWidth)
		{
			for (FOverlapResult& OverlapRes : OverlapResults)
			{
				if (ABlock* BlockToDestroy = Cast<ABlock>(OverlapRes.GetActor()))
				{
					BlockToDestroy->Destroy();
				}
			}
		}
	}
}

void AMainCharacter::SetCameraToCenter()
{
	FVector NewLocation = GetActorLocation();
	NewLocation += FVector{ 0.0f, BlockLength * (float)(BoardWidth / 2), BlockLength * (float)(BoardHeight / 2) };
	CameraComponent->SetWorldLocation(NewLocation);
}

void AMainCharacter::ResetFocusLocation()
{
	FVector NewLocation = GetActorLocation();
	NewLocation += FVector{ 0.0f, BlockLength * (float)(BoardWidth / 2), BlockLength * (float)(BoardHeight - 1) };
	BlockFocusLocation = NewLocation;
}

void AMainCharacter::SetBoardSize(int nWidth, int nHeight)
{
	FBlockTypeInfoRow* BlockTypeRow = BlockTypeInfoTable->FindRow<FBlockTypeInfoRow>(TEXT("White"), TEXT("RowNotFound"));

	BoardWidth = nWidth;
	BoardHeight = nHeight;

	CameraComponent->SetOrthoWidth(BlockLength * nWidth + BoardWidthPadding);
	CameraComponent->SetAspectRatio(CameraComponent->OrthoWidth / (BlockLength * nHeight + BoardHeightPadding));
	SetCameraToCenter();

	for (int i = -1; i < nWidth + 1; ++i)
	{
		for (int j = -1; j < nHeight; ++j)
		{
			if (j == -1 || (i == -1 || i == nWidth))
			{
				ABlock* NewBoundary = GetWorld()->SpawnActor<ABlock>(BlockClass);
				NewBoundary->SetColor(BlockTypeRow->Material);
				NewBoundary->SetActorLocation(GetActorLocation());
				NewBoundary->AddActorWorldOffset(FVector{0.0f, BlockLength * i, BlockLength * j });
			}
		}
	}

	ResetFocusLocation();
}

void AMainCharacter::DestroyControllingBlocks()
{
	for (ABlock* ControllingBlock : ControllingBlocks)
	{
		ControllingBlock->Destroy();
	}

	ControllingBlocks.Empty();
}

bool AMainCharacter::SpawnControllingBlocks(EBlockShapeAndColor Color, FVector PivotPosition, int NewDirection)
{
	ControllingBlockShapeAndColor = Color;

	ControllingBlocks.Empty();

	FBlockTypeInfoRow& FoundRow = FindBlockInfoRowByEnum(Color);

	if (Color == EBlockShapeAndColor::White || Color == EBlockShapeAndColor::Black)
	{
		return false;
	}

	for (int i = 0; i < 4; ++i)
	{
		ABlock* NewBlock = GetWorld()->SpawnActor<ABlock>(BlockClass);
		NewBlock->SetActorLocation(PivotPosition);
		switch (NewDirection)
		{
		case 0:
			NewBlock->AddActorWorldOffset(FoundRow.DirectionA[i]);
			break;
		case 1:
			NewBlock->AddActorWorldOffset(FoundRow.DirectionB[i]);
			break;
		case 2:
			NewBlock->AddActorWorldOffset(FoundRow.DirectionC[i]);
			break;
		case 3:
			NewBlock->AddActorWorldOffset(FoundRow.DirectionD[i]);
			break;
		default:
			break;
		}
		NewBlock->SetColor(FoundRow.Material);
		ControllingBlocks.Add(NewBlock);
	}

	return true;
}

void AMainCharacter::RotateControllingBlock()
{
	ControllingBlockDirection = ++ControllingBlockDirection % 4;

	DestroyControllingBlocks();
	SpawnControllingBlocks(ControllingBlockShapeAndColor, BlockFocusLocation, ControllingBlockDirection);
}

void AMainCharacter::MoveControllingBlockLeft()
{
	BlockFocusLocation.Y -= 40.0f;

	DestroyControllingBlocks();
	SpawnControllingBlocks(ControllingBlockShapeAndColor, BlockFocusLocation, ControllingBlockDirection);
}

void AMainCharacter::MoveControllingBlockRight()
{
	BlockFocusLocation.Y += 40.0f;

	DestroyControllingBlocks();
	SpawnControllingBlocks(ControllingBlockShapeAndColor, BlockFocusLocation, ControllingBlockDirection);
}

void AMainCharacter::MoveControllingBlockDown()
{
	BlockFocusLocation.Z -= 40.0f;

	bool bIsHit = false;
	FBlockTypeInfoRow& FoundRow = FindBlockInfoRowByEnum(ControllingBlockShapeAndColor);
	TArray<FOverlapResult> OverlapResults{};

	for (int i = 0; i < 4; ++i)
	{
		TArray<FVector>* DirectionToApply{};
		switch (ControllingBlockDirection)
		{
		case 0:
			DirectionToApply = &FoundRow.DirectionA;
			break;
		case 1:
			DirectionToApply = &FoundRow.DirectionB;
			break;
		case 2:
			DirectionToApply = &FoundRow.DirectionC;
			break;
		case 3:
			DirectionToApply = &FoundRow.DirectionD;
			break;
		default:
			break;
		}

		FVector OverlapCheckLocation = BlockFocusLocation + (*DirectionToApply)[i];
		GetWorld()->OverlapMultiByObjectType(OverlapResults, OverlapCheckLocation, FQuat{}, FCollisionObjectQueryParams::AllDynamicObjects, FCollisionShape::MakeSphere(BlockLength / 2.0f), FCollisionQueryParams::DefaultQueryParam);
	}

	for (FOverlapResult& OverlapResult : OverlapResults)
	{
		ABlock* CollidedSomething = reinterpret_cast<ABlock*>(OverlapResult.GetActor());

		if (!ControllingBlocks.Contains(CollidedSomething) && CollidedSomething != reinterpret_cast<ABlock*>(this))
		{
			bIsHit = true;
			break;
		}
	}

	if (bIsHit)
	{
		CheckBlockComplete();
		SpawnRandomNewBlock();
	}
	else
	{
		DestroyControllingBlocks();
		SpawnControllingBlocks(ControllingBlockShapeAndColor, BlockFocusLocation, ControllingBlockDirection);
	}
}

void AMainCharacter::MoveControllingBlockUp()
{
	BlockFocusLocation.Z += 40.0f;

	DestroyControllingBlocks();
	SpawnControllingBlocks(ControllingBlockShapeAndColor, BlockFocusLocation, ControllingBlockDirection);
}
