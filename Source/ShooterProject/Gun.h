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

	// �޽�
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshCom_;

	// �ѱ� ��ƼŬ
	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlashParticle_;

	UPROPERTY(EditAnywhere)
	UParticleSystem* GunFireParticle_;

	// ���� ����Ʈ
	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSoundCue_;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSoundConcreteCue_;

	// ��Ÿ�
	UPROPERTY(EditAnywhere)
	float MaxRange_ = 10000.f;

	// ������
	UPROPERTY(EditAnywhere)
	float Damage_ = 10.f;

	// ���� ����Ʈ���̽�
	bool GunTrace(FHitResult& _HitResult, FVector& _ShotDir);

	AController* GetOwnerController() const;

	// �Ѿ˰���
	UPROPERTY(EditAnywhere)
	int MaxAmmo_ = 30;

	UPROPERTY(EditAnywhere)
	int Ammo_;

	// �ݵ� ��ġ ����
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

	// �ݵ�ġ
	FRotator ResultRecoilRotation_ = FRotator::ZeroRotator;

	FTimerHandle RecoilTimaerManager_;
};
