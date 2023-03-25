// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "ShooterCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <GameFramework/Actor.h>

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn_ = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (AIBehavior_ != nullptr)
	{
		RunBehaviorTree(AIBehavior_);

// BT의 블랙보드에 값을 설정
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AShooterAIController::IsDead() const
{
	AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());

	if (ControlledCharacter != nullptr) return ControlledCharacter->IsDeath();

	return true;
}
