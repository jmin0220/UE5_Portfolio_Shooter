// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile_SkillE.generated.h"

UCLASS()
class SHOOTERPROJECT_API AProjectile_SkillE : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile_SkillE();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	AController* GetOwnerController() const;

	void Explode();

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BaseMesh_;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovementCom_;

	UPROPERTY(VisibleAnywhere)
	class URadialForceComponent* RadialForceComponent_;

	UPROPERTY(EditAnywhere, category = "Status")
	float Damage_ = 80.f;

	// SpawnEmitter���� ����ϱ� ���� ��ƼŬ
	UPROPERTY(EditAnywhere)
	UParticleSystem* ExplosionParticles_;

	// ��ƼŬ ������Ʈ
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* TrailParticles_;

	// �߻� ���� ����Ʈ
	UPROPERTY(EditAnywhere)
	class USoundBase* LaunchSound_;

	// Ÿ�� ���� ����Ʈ
	UPROPERTY(EditAnywhere)
	class USoundBase* ExplosionSound_;

	// ī�޶� ����ũ
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass_;

	FTimerHandle ExplodeTimer_;


};
