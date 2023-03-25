// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include <TimerManager.h>
#include <Blueprint/UserWidget.h>

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	CrossHairScreen_->RemoveFromViewport();

	if (bIsWinner)
	{
		UUserWidget* WinScreen = CreateWidget(this, WinScreenWidget_);

		if (WinScreen != nullptr)
		{
			WinScreen->AddToViewport();
		}
	}
	else
	{
		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenWidget_);

		if (LoseScreen != nullptr)
		{
			LoseScreen->AddToViewport();
			GetWorldTimerManager().SetTimer(RestartTimer_, this, &APlayerController::RestartLevel, RestartDelay_);
		}
	}
}

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 크로스헤어 생성
	CrossHairScreen_ = CreateWidget(this, CrossHairWidget_);
	CrossHairScreen_->AddToViewport();
}
