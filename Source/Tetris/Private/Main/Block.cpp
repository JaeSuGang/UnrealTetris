
#include "Main/Block.h"

#include "Components/StaticMeshComponent.h"

ABlock::ABlock()
{
	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	RootComponent = DefaultSceneComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

void ABlock::BeginPlay()
{
	Super::BeginPlay();
	
}

