

#include "Main/MainCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraActor.h"
#include "EngineUtils.h"

#include "Main/MainPlayerController.h"

AMainCharacter::AMainCharacter()
{
	GetCharacterMovement()->GravityScale = 0.0f;

	GetCapsuleComponent()->SetCapsuleSize(10.0f, 10.0f);
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMainCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	for (TActorIterator<ACameraActor> CameraActorIter{ GetWorld() }; CameraActorIter; ++CameraActorIter)
	{
		if (CameraActorIter->ActorHasTag(TEXT("Camera0")))
		{
			CameraActor = *CameraActorIter;
			break;
		}
	}

	if (AMainPlayerController* MPC = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		MPC->OnPossessDelegate.AddDynamic(this, &AMainCharacter::ViewAsCameraActor);
	}
}

void AMainCharacter::ViewAsCameraActor()
{
	if (APlayerController* PC = Cast<APlayerController>(GetOwner()))
	{
		PC->SetViewTarget(CameraActor);
	}
}

