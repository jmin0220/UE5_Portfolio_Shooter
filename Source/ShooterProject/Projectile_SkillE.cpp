// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_SkillE.h"
#include "ShooterCharacter.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>
#include <Camera/CameraShakeBase.h>
#include <PhysicsEngine/RadialForceComponent.h>

// Sets default values
AProjectile_SkillE::AProjectile_SkillE()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh_ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	RootComponent = BaseMesh_;

	ProjectileMovementCom_ = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementCom"));
	ProjectileMovementCom_->MaxSpeed = 600.f;

	TrailParticles_ = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailParticles"));
	TrailParticles_->SetupAttachment(RootComponent);

	RadialForceComponent_ = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForceComponent_->SetupAttachment(RootComponent);

	RadialForceComponent_->Radius = 300.0f;
	RadialForceComponent_->ImpulseStrength = 1500.0f;
	RadialForceComponent_->bImpulseVelChange = true;
	RadialForceComponent_->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AProjectile_SkillE::BeginPlay()
{
	Super::BeginPlay();

	// �߻�Ҹ� ���
	if (LaunchSound_)
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound_, GetActorLocation());

	GetWorldTimerManager().SetTimer(ExplodeTimer_, this, &AProjectile_SkillE::Explode, 3.f, false);
}

// Called every frame
void AProjectile_SkillE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AController* AProjectile_SkillE::GetOwnerController() const
{
	// ��Ʈ�ѷ��� ������� Pawn���� ĳ����
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)  return nullptr;

	return OwnerPawn->GetController();
}

void AProjectile_SkillE::Explode()
{

	// �浹�� ���͸� ����ϱ� ���� ����
	TArray<FOverlapResult> tmpOverlapResult;
	FCollisionObjectQueryParams tmpObjectQueryParams;
	tmpObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	
	// ���߹ݰ�� ������ ũ���� sphere����
	FCollisionShape tmpOverlapSphere = FCollisionShape::MakeSphere(RadialForceComponent_->Radius);
	
	// �������� ���͸� �˻�
	GetWorld()->OverlapMultiByObjectType(tmpOverlapResult, RadialForceComponent_->GetComponentLocation(), FQuat::Identity, tmpObjectQueryParams, tmpOverlapSphere);


	// �ߺ�ó�� ������ ���� 
	TArray<AShooterCharacter*> checkedChar;
	AController* OwnerController = GetOwnerController();

	for (const FOverlapResult overlapResult : tmpOverlapResult)
	{
		AShooterCharacter* tmpShooter = Cast<AShooterCharacter>(overlapResult.GetActor());
				
		if (tmpShooter != nullptr && checkedChar.Contains(tmpShooter) == false)
		{
			FDamageEvent DamageEvent;
			tmpShooter->TakeDamage(Damage_, DamageEvent, this->GetInstigatorController(), nullptr);

			checkedChar.Add(tmpShooter);
		}
	}

	// ������Ʈ ���� ����
	RadialForceComponent_->FireImpulse();

	if (ExplosionParticles_)
	{
		// Hit Particle�� ����
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticles_, GetActorLocation(), GetActorRotation());
	}

	Destroy();
}

