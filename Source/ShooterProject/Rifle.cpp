// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"
#include "ShooterCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <TimerManager.h>

ARifle::ARifle()
{
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(ShotTimer_, this, &ARifle::Shot, ShotDelay_, true);
	GetWorldTimerManager().PauseTimer(ShotTimer_);
}

void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsHidden())
	{
		IsShot_ = false;
		return;
	}

}

void ARifle::PullTrigger()
{
	IsShot_ = !IsShot_;

	if (IsShot_)
	{
		GetWorldTimerManager().UnPauseTimer(ShotTimer_);
	}
	else
	{
		GetWorldTimerManager().PauseTimer(ShotTimer_);
	}
}

void ARifle::Shot()
{
	if (Ammo_ <= 0)
	{
		Reload();
		GetWorldTimerManager().PauseTimer(ShotTimer_);

		return;
	}

	UGameplayStatics::SpawnEmitterAttached(MuzzleFlashParticle_, SkeletalMeshCom_, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSoundCue_, SkeletalMeshCom_, TEXT("MuzzleFlashSocket"));

	FHitResult HitResult;
	FVector ShotDir = FVector::ZeroVector;

	if (GunTrace(HitResult, ShotDir))
	{
		AController* OwnerController = GetOwnerController();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GunFireParticle_, HitResult.Location, ShotDir.Rotation(), true);

		FPointDamageEvent DamageEvent(Damage_, HitResult, ShotDir, nullptr);

		AActor* HitActor = HitResult.GetActor();
		if (HitActor == nullptr)
		{
			return;
		}

		// Shooter캐릭터가 아닌것에 충돌했을 경우의 사운드
		if (Cast<AShooterCharacter>(HitActor) == nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), MuzzleSoundConcreteCue_, HitResult.ImpactPoint);
		}

		HitActor->TakeDamage(Damage_, DamageEvent, OwnerController, GetOwner());
	}

	Ammo_--;
	AddRecoil();
}
