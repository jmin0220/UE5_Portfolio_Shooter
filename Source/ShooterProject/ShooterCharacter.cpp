// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "KillEmAllGameMode.h"
#include "ShooterProjectGameModeBase.h"
#include "ShooterAnimInstace.h"
#include "Projectile_SkillE.h"
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>
#include <Animation/AnimSingleNodeInstance.h>
#include <Animation/AnimNode_StateMachine.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmCom_ = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmCom"));
	SpringArmCom_->SetupAttachment(RootComponent);

	CameraCom_ = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraCom"));
	CameraCom_->SetupAttachment(SpringArmCom_);

	ProjectileSpawnPoint_ = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint_->SetupAttachment(RootComponent);

	// ���ε� �ִϸ��̼� ��Ÿ�ָ� ����
	ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadMontage(
		TEXT("AnimMontage'/Game/Blueprint/AM_Ability_E.AM_Ability_E'"));
	// ��ų �ִϸ��̼� ��Ÿ�ָ� ����
	ConstructorHelpers::FObjectFinder<UAnimMontage> AbilityQ_Montage(
		TEXT("AnimMontage'/Game/Blueprint/AM_Ability_Q_Fire.AM_Ability_Q_Fire'"));

	if (ReloadMontage.Succeeded())
	{
		ReloadMontage_ = ReloadMontage.Object;
	}
	if (AbilityQ_Montage.Succeeded())
	{
		AbilityQ_Montage_ = AbilityQ_Montage.Object;
	}
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (this == UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) IsPlayer_ = true;

	UShooterAnimInstace* tmpAnimInstance = Cast<UShooterAnimInstace>(GetMesh()->GetAnimInstance());
	tmpAnimInstance->SetPlayerPawn(this);

	//�������� �ؾ��� ó��
	// ü�� �ʱ�ȭ
	Health_ = MaxHealth_;

	// ���⸦ �����ϰ�, ���� ��� �ִ� ���⸦ ����
	GunArr_.Reserve(3);
	for (TSubclassOf<AGun> tmpGun : GunClassArr_)
	{
		GunArr_.Add(GetWorld()->SpawnActor<AGun>(tmpGun));
	}

	for (AGun* tmpGun : GunArr_)
	{
		if (tmpGun == nullptr) continue;

		tmpGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		tmpGun->SetOwner(this);
		tmpGun->SetActorHiddenInGame(true);
		tmpGun->PrimaryActorTick.SetTickFunctionEnable(false);
	}

	gunState = GunS::Gun1;
	GunArr_[static_cast<int>(gunState)]->SetActorHiddenInGame(false);
	GunArr_[static_cast<int>(gunState)]->PrimaryActorTick.SetTickFunctionEnable(true);
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ī�޶� �� ������Ʈ
	ZoomUpdate(DeltaTime);

	// �ֱ� �ִϸ��̼� ������Ʈ�� ���
	UAnimInstance* CurAnimInstance = GetMesh()->GetAnimInstance();
	
	if (CurAnimInstance != nullptr)
	{
		if (IsPlayer_ == false) return;

		// IsMovable_ = true;
	}
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// �������� ü�º��� �� ���� ��� ü�¸�ŭ�� �������� �� �� �ֵ���
	DamageApplied = FMath::Min(Health_, DamageApplied);
	Health_ -= DamageApplied;

	// Deathó��
	if (Health_ <= 0.f) IsDeath_ = true;

	FString name = GetName();
	UE_LOG(LogTemp, Warning, TEXT("%s Health >> %f"), *name, Health_);

	if (IsDeath())
	{
		// ���Ӹ�忡 Pawn�� �׾����� �˸�
		AKillEmAllGameMode* GameMode = GetWorld()->GetAuthGameMode<AKillEmAllGameMode>();

		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}

		// ��Ʈ�ѷ��� �и�
		DetachFromControllerPendingDestroy();
		// ĸ���� �ݸ����� ����
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	} 

	return Health_;
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooterCharacter::LookRightRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AShooterCharacter::GunFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &AShooterCharacter::GunFire);
	PlayerInputComponent->BindAction(TEXT("SwitchWeapon1"), EInputEvent::IE_Pressed, this, &AShooterCharacter::SwitchWeapon1);
	PlayerInputComponent->BindAction(TEXT("SwitchWeapon2"), EInputEvent::IE_Pressed, this, &AShooterCharacter::SwitchWeapon2);
	PlayerInputComponent->BindAction(TEXT("SwitchWeapon3"), EInputEvent::IE_Pressed, this, &AShooterCharacter::SwitchWeapon3);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AShooterCharacter::PlayReloadAnim);

	PlayerInputComponent->BindAction(TEXT("Zoom"), EInputEvent::IE_Pressed, this, &AShooterCharacter::ZoomIn);
	PlayerInputComponent->BindAction(TEXT("Zoom"), EInputEvent::IE_Released, this, &AShooterCharacter::ZoomOut);

	// ���� �е� �Է��� ���� �Լ�
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);
}

inline int AShooterCharacter::GetMaxAmmo() const
{
	return GunArr_[static_cast<int>(gunState)]->GetMaxAmmo();
}

inline int AShooterCharacter::GetAmmo() const
{
	return GunArr_[static_cast<int>(gunState)]->GetAmmo();
}

bool AShooterCharacter::GetIsReload() const
{
	return IsReload_;
}

bool AShooterCharacter::GetIsUseSkillE() const
{
	return IsUseSkillE_;
}

void AShooterCharacter::PlayReloadAnim()
{
	IsReload_ = true;
	GetMesh()->GetAnimInstance()->Montage_Play(ReloadMontage_);
}

void AShooterCharacter::SpawnProjectile()
{
	AProjectile_SkillE* tmpProjectile = GetWorld()->SpawnActor<AProjectile_SkillE>(Skill_E_Projectile_, ProjectileSpawnPoint_->GetComponentLocation(), ProjectileSpawnPoint_->GetComponentRotation());
	
	if (tmpProjectile == nullptr) return;
	tmpProjectile->SetOwner(this);
}

void AShooterCharacter::AddHealth(float _Health)
{
	Health_ += _Health;

	// �ִ�ü�� �ʰ�
	if (Health_ >= MaxHealth_)
	{
		Health_ = MaxHealth_;
	}
}

void AShooterCharacter::GunReload()
{
	IsReload_ = false;
	GunArr_[static_cast<int>(gunState)]->ResetAmmo();
}

void AShooterCharacter::MoveForward(float _AxisValue)
{
	if (!IsMovable_) return;

	AddMovementInput(GetActorForwardVector() * _AxisValue);
}

void AShooterCharacter::MoveRight(float _AxisValue)
{
	if (!IsMovable_) return;

	AddMovementInput(GetActorRightVector() * _AxisValue);
}

void AShooterCharacter::LookUpRate(float _AxisValue)
{
	AddControllerPitchInput(_AxisValue * RotationRate_ * UGameplayStatics::GetWorldDeltaSeconds(this));
}

void AShooterCharacter::LookRightRate(float _AxisValue)
{
	AddControllerYawInput(_AxisValue * RotationRate_ * UGameplayStatics::GetWorldDeltaSeconds(this));
}

void AShooterCharacter::Jump()
{
	if (!IsMovable_) return;

	Super::Jump();
}

void AShooterCharacter::SwitchWeapon1()
{
	if (!IsMovable_) return;

	if (GunArr_[static_cast<int>(GunS::Gun1)] == nullptr) return;

	// ���� ������� ���⸦ ����
	GunArr_[static_cast<int>(gunState)]->SetActorHiddenInGame(true);
	GunArr_[static_cast<int>(gunState)]->PrimaryActorTick.SetTickFunctionEnable(false);

	// ������ ���⸦ ǥ��
	gunState = GunS::Gun1;
	GunArr_[static_cast<int>(gunState)]->SetActorHiddenInGame(false);
	GunArr_[static_cast<int>(gunState)]->PrimaryActorTick.SetTickFunctionEnable(true);
}

void AShooterCharacter::SwitchWeapon2()
{
	if (!IsMovable_) return;

	if (GunArr_[static_cast<int>(GunS::Gun2)] == nullptr) return;

	GunArr_[static_cast<int>(gunState)]->SetActorHiddenInGame(true);
	GunArr_[static_cast<int>(gunState)]->PrimaryActorTick.SetTickFunctionEnable(false);

	// ������ ���⸦ ǥ��
	gunState = GunS::Gun2;
	GunArr_[static_cast<int>(gunState)]->SetActorHiddenInGame(false);
	GunArr_[static_cast<int>(gunState)]->PrimaryActorTick.SetTickFunctionEnable(true);
}

void AShooterCharacter::SwitchWeapon3()
{
	IsMovable_ = false;
	IsUseSkillE_ = true;
	GetMesh()->GetAnimInstance()->Montage_Play(AbilityQ_Montage_);
}

void AShooterCharacter::ZoomIn()
{
	IsZoomIn_ = true;
}

void AShooterCharacter::ZoomOut()
{
	IsZoomIn_ = false;
}

void AShooterCharacter::ZoomUpdate(float _DeltaTime)
{
	// ����
	if (IsZoomIn_ == true && ZoomDeltaTime_ < 1.f)
	{
		ZoomDeltaTime_ += _DeltaTime * 10;
		SpringArmCom_->TargetArmLength = FMath::Lerp(ZoomOutLength_, ZoomInLength_, ZoomDeltaTime_);
	}
	// �ܾƿ�
	else if(IsZoomIn_ == false && ZoomDeltaTime_ > 0.f)
	{
		ZoomDeltaTime_ -= _DeltaTime * 10;
		SpringArmCom_->TargetArmLength = FMath::Lerp(ZoomOutLength_, ZoomInLength_, ZoomDeltaTime_);
	}

	// ������ �ʰ����� ��� ���� ������
	if (SpringArmCom_->TargetArmLength < ZoomInLength_)
	{
		SpringArmCom_->TargetArmLength = ZoomInLength_;
	}
	else if (SpringArmCom_->TargetArmLength > ZoomOutLength_)
	{
		SpringArmCom_->TargetArmLength = ZoomOutLength_;
	}
}

void AShooterCharacter::GunFire()
{
	if (!IsMovable_) return;

	GunArr_[static_cast<int>(gunState)]->PullTrigger();
}
