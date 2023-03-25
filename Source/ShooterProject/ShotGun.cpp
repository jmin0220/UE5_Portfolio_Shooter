// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotGun.h"
#include "ShooterCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <DrawDebugHelpers.h>

AShotGun::AShotGun()
{
}

void AShotGun::BeginPlay()
{
	Super::BeginPlay();
}

void AShotGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsHidden())
	{
		IsShot_ = false;
		return;
	}
}

void AShotGun::PullTrigger()
{
	IsShot_ = !IsShot_;

	if (IsShot_)
	{
		if (Ammo_ <= 0)
		{
			Reload();
			return;
		}

		UGameplayStatics::SpawnEmitterAttached(MuzzleFlashParticle_, SkeletalMeshCom_, TEXT("MuzzleFlashSocket"));
		UGameplayStatics::SpawnSoundAttached(MuzzleSoundCue_, SkeletalMeshCom_, TEXT("MuzzleFlashSocket"));

		FHitResult HitResult;
		FVector ShotDir = FVector::ZeroVector;

		for (int i = 0; i < 5; i++)
		{
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
		}

		Ammo_ -= 5;
	}

	AddRecoil();
}

bool AShotGun::GunTrace(FHitResult& _HitResult, FVector& _ShotDir)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return false;

	// BulletTrace의 시작점
	FVector ActorLocation = GetActorLocation();
	FRotator ActorRotator = GetActorRotation();

	// 카메라의 위치에서 발사 포인트 시작
	OwnerController->GetPlayerViewPoint(ActorLocation, ActorRotator);

	// BulletTrace의 끝점
	// 산탄을 위한 각도 조절
	ActorRotator.Add(FMath::RandRange(-LaunchAngle_, LaunchAngle_), 0.f, 0.f);
	ActorRotator.Add(0.f, FMath::RandRange(-LaunchAngle_, LaunchAngle_), 0.f);
	FVector End = ActorLocation + ActorRotator.Vector() * MaxRange_;
	//DrawDebugLine(GetWorld(), ActorLocation, End, FColor::Red, false, 2.f, 0, 5.f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	FVector ShotDir = -ActorRotator.Vector();

	UE_LOG(LogTemp, Warning, TEXT("ActorRotator: Pitch: %f, Yaw: %f, Roll: %f"), ActorRotator.Pitch, ActorRotator.Yaw, ActorRotator.Roll);
	

	// 레인트레이스의 결과
	return GetWorld()->LineTraceSingleByChannel(_HitResult, ActorLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params);

}
