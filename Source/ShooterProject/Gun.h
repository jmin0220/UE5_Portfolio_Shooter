// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SHOOTERPROJECT_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PullTrigger() {};

	inline int GetMaxAmmo() const
	{
		return MaxAmmo_;
	}

	inline int GetAmmo() const
	{
		return Ammo_;
	}

	inline void ResetAmmo()
	{
		Ammo_ = MaxAmmo_;
	}

	void Reload();

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root_;

	// 메쉬
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshCom_;

	// 총기 파티클
	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlashParticle_;

	UPROPERTY(EditAnywhere)
	UParticleSystem* GunFireParticle_;

	// 사운드 이펙트
	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSoundCue_;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSoundConcreteCue_;

	// 사거리
	UPROPERTY(EditAnywhere)
	float MaxRange_ = 10000.f;

	// 데미지
	UPROPERTY(EditAnywhere)
	float Damage_ = 10.f;

	// 총의 레인트레이스
	bool GunTrace(FHitResult& _HitResult, FVector& _ShotDir);

	AController* GetOwnerController() const;

	// 총알갯수
	UPROPERTY(EditAnywhere)
	int MaxAmmo_ = 30;

	UPROPERTY(EditAnywhere)
	int Ammo_;

	// 반동 수치 조절
	UPROPERTY(EditAnywhere, category = "Recoil")
	float PitchRecoilIntensity_ = 1.f;

	UPROPERTY(EditAnywhere, category = "Recoil")
	float YawRecoilIntensity_ = 1.f;

	UPROPERTY(EditAnywhere, category = "Recoil")
	float RecoilSpeed_ = 1.f;

	UPROPERTY(EditAnywhere, category = "Recoil")
	float ResetRecoilTimer_ = 0.3;

	void AddRecoil();
	void ResetRecoil();

	// 반동치
	FRotator ResultRecoilRotation_ = FRotator::ZeroRotator;

	FTimerHandle RecoilTimaerManager_;
};
