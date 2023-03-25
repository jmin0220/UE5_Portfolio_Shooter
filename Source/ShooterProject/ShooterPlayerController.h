// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERPROJECT_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WinScreenWidget_;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LoseScreenWidget_;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CrossHairWidget_;

	UPROPERTY(EditAnywhere)
	float RestartDelay_ = 5.f;

	FTimerHandle RestartTimer_;

	UPROPERTY()
	UUserWidget* CrossHairScreen_;
};
