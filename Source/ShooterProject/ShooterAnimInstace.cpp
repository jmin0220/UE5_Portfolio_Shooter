// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstace.h"
#include "ShooterCharacter.h"
#include <Kismet/GameplayStatics.h>

void UShooterAnimInstace::SetPlayerPawn(AShooterCharacter* _PlayerPawn)
{
	ShooterCharacter_ = _PlayerPawn;
}

void UShooterAnimInstace::AnimNotify_EndReloadAnim()
{
	ShooterCharacter_->GunReload();
}

void UShooterAnimInstace::AnimNotify_ShotStart()
{
	// ���� �߻��ϴ� ����
	ShooterCharacter_->SpawnProjectile();
}

void UShooterAnimInstace::AnimNotify_EndSkillEAnim()
{
	ShooterCharacter_->SetUseSkillandMovableFlg(false);
}
