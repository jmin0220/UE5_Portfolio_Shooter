// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstace.generated.h"

/**
 * 
 */
class AShooterCharacter;
UCLASS()
class SHOOTERPROJECT_API UShooterAnimInstace : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	void SetPlayerPawn(AShooterCharacter* _PlayerPawn);

private:
	AShooterCharacter* ShooterCharacter_;

	UFUNCTION()
	void AnimNotify_EndReloadAnim();

	UFUNCTION()
	void AnimNotify_ShotStart();

	UFUNCTION()
	void AnimNotify_EndSkillEAnim();
};
