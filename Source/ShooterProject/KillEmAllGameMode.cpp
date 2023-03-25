// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "ShooterAIController.h"
#include <EngineUtils.h>
#include <GameFramework/Controller.h>

void AKillEmAllGameMode::PawnKilled(APawn* _KilledPawn)
{
	Super::PawnKilled(_KilledPawn);

	APlayerController* PlayerController = Cast<APlayerController>(_KilledPawn->GetController());

	// �÷��̾ ���� ���
	if (PlayerController != nullptr)
	{
		EndGame(false);
	}

	// ��� AI��Ʈ�ѷ��� ���� ���
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
	// world�� ��� Controller�׷��� ��ȯ
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool isPlayerCon = Controller->IsPlayerController() == _IsPlayerWinnder;
		Controller->GameHasEnded(Controller->GetPawn(), isPlayerCon);
	}
}
