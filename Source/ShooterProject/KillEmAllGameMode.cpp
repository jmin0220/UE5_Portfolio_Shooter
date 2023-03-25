// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "ShooterAIController.h"
#include <EngineUtils.h>
#include <GameFramework/Controller.h>

void AKillEmAllGameMode::PawnKilled(APawn* _KilledPawn)
{
	Super::PawnKilled(_KilledPawn);

	APlayerController* PlayerController = Cast<APlayerController>(_KilledPawn->GetController());

	// 플레이어가 죽은 경우
	if (PlayerController != nullptr)
	{
		EndGame(false);
	}

	// 모든 AI컨트롤러가 죽은 경우
	for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
	{
		if (!Controller->IsDead())
		{
			return;
		}
	}

	EndGame(true);
}

void AKillEmAllGameMode::EndGame(bool _IsPlayerWinnder)
{
	// world의 모든 Controller그룹을 반환
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool isPlayerCon = Controller->IsPlayerController() == _IsPlayerWinnder;
		Controller->GameHasEnded(Controller->GetPawn(), isPlayerCon);
	}
}
