// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "ShooterCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
	NodeName = TEXT("Can See Player");
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (AIController == nullptr || Player == nullptr) return;

	if (AIController->LineOfSightTo(Player))
	{
		// BT의 블랙보드에 값을 설정
		AIController->GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), Player);
		//AIController->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Player->GetActorLocation());
	}
	else
	{
		AIController->GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
}
