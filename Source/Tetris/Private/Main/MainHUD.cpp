// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/MainHUD.h"

#include "Blueprint/UserWidget.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* StarterWidget = CreateWidget<UUserWidget>(GetWorld(), StarterWidgetClass);
	StarterWidget->AddToViewport();
}
