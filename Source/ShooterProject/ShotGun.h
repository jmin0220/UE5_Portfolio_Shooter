// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "ShotGun.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERPROJECT_API AShotGun : public AGun
{
	GENERATED_BODY()
	
public:
	AShotGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PullTrigger() override;

	// 총의 레인트레이스
	bool GunTrace(FHitResult& _HitResult, FVector& _ShotDir);
private:
	UPROPERTY(EditAnywhere)
	float LaunchAngle_ = 3.f;

	bool IsShot_ = false;
};
