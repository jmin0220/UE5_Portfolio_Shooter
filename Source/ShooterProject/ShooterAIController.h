// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERPROJECT_API AShooterAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsDead() const;
private:
	APawn* PlayerPawn_;

	UPROPERTY(EditDefaultsOnly)
	float AcceptanceRadius_ = 100.f;

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior_;
};
