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

	// 발사소리 재생
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
	// 컨트롤러를 얻기위해 Pawn으로 캐스팅
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)  return nullptr;

	return OwnerPawn->GetController();
}

void AProjectile_SkillE::Explode()
{

	// 충돌한 액터를 취득하기 위한 변수
	TArray<FOverlapResult> tmpOverlapResult;
	FCollisionObjectQueryParams tmpObjectQueryParams;
	tmpObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	
	// 폭발반경과 동일한 크기의 sphere생성
	FCollisionShape tmpOverlapSphere = FCollisionShape::MakeSphere(RadialForceComponent_->Radius);
	
	// 오버랩된 액터를 검색
	GetWorld()->OverlapMultiByObjectType(tmpOverlapResult, RadialForceComponent_->GetComponentLocation(), FQuat::Identity, tmpObjectQueryParams, tmpOverlapSphere);


	// 중복처리 방지를 위한 
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

	// 오브젝트 폭발 물리
	RadialForceComponent_->FireImpulse();

	if (ExplosionParticles_)
	{
		// Hit Particle을 생성
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticles_, GetActorLocation(), GetActorRotation());
	}

	Destroy();
}

