// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "Rifle.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERPROJECT_API ARifle : public AGun
{
	GENERATED_BODY()
	
public:
	ARifle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PullTrigger() override;

private:
	void Shot();

	bool IsShot_ = false;

	FTimerHandle ShotTimer_;

	UPROPERTY(EditAnywhere)
	float ShotDelay_ = 0.1f;
};
