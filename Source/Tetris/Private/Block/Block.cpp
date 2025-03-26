
#include "Block/Block.h"

#include "Components/StaticMeshComponent.h"

ABlock::ABlock()
{
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	RootComponent = DefaultSceneComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

void ABlock::BeginPlay()
{
	Super::BeginPlay();


}

void ABlock::SetColor(UMaterialInterface* NewMaterial)
{
	StaticMeshComponent->SetMaterial(0, NewMaterial);
}

