// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

enum class GunS
{
	Gun1 = 0,
	Gun2,
	Gun3,
};

class AGun;
UCLASS()
class SHOOTERPROJECT_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void GunFire();

	// BlueprintPure > �������� ���� �������Ʈ �Լ��� ����
	UFUNCTION(BlueprintPure)
	inline bool IsDeath() const
	{
		return IsDeath_;
	}

	UFUNCTION(BlueprintPure)
	inline float GetHealthPercent() const
	{
		return Health_ / MaxHealth_;
	}

	// ���� ��� �ִ� ����� Ammo�� ȹ��
	UFUNCTION(BlueprintPure)
	int GetMaxAmmo() const;

	UFUNCTION(BlueprintPure)
	int GetAmmo() const;

	UFUNCTION(BlueprintPure)
	bool GetIsReload() const;

	UFUNCTION(BlueprintPure)
	bool GetIsUseSkillE() const;

	void GunReload();

	void PlayReloadAnim();

	inline void SetUseSkillandMovableFlg(bool _Flg)
	{
		IsUseSkillE_ = _Flg;
		IsMovable_ = !_Flg;
	}

	void SpawnProjectile();

	void AddHealth(float _Health);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraCom_;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmCom_;

	// ��ų�� ���� �Ѿ��� ������ ����Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "Component Infomation", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Component Positions", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectile_SkillE> Skill_E_Projectile_;



	void MoveForward(float _AxisValue);
	void MoveRight(float _AxisValue);

	void LookUpRate(float _AxisValue);
	void LookRightRate(float _AxisValue);

	void Jump();

	void SwitchWeapon1();
	void SwitchWeapon2();
	void SwitchWeapon3();

	void ZoomIn();
	void ZoomOut();
	void ZoomUpdate(float _DeltaTime);


	GunS gunState;

	UPROPERTY(EditAnywhere)
	float RotationRate_ = 10.f;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AGun>> GunClassArr_;

	UPROPERTY(EditDefaultsOnly)
	TArray<AGun*> GunArr_;

	int SelectedGun_ = 0;

	// HP���� ����
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth_ = 100.f;

	UPROPERTY(VisibleAnywhere)
	float Health_;

	// ������ ���
	UAnimMontage* ReloadMontage_;
	// ��ų ���
	UAnimMontage* AbilityQ_Montage_;

	bool IsDeath_ = false;
	bool IsMovable_ = true;
	bool IsReload_ = false;
	bool IsUseSkillE_ = false;

	bool IsPlayer_ = false;

	float ZoomInLength_ = 0.f;
	float ZoomOutLength_ = 200.f;
	float ZoomDeltaTime_ = 0.f;
	bool IsZoomIn_ = false;
};