// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterProjectGameModeBase.h"
#include "KillEmAllGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERPROJECT_API AKillEmAllGameMode : public AShooterProjectGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PawnKilled(APawn* _KilledPawn) override;

private:
	void EndGame(bool _IsPlayerWinnder);
};
