// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include <Components/SkeletalMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include <DrawDebugHelpers.h>
#include "ShooterCharacter.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root_ = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root_;

	SkeletalMeshCom_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComponent"));
	SkeletalMeshCom_->SetupAttachment(Root_);

	Ammo_ = MaxAmmo_;
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	// �ݵ��� ���ִ� Ÿ�̸�
	GetWorldTimerManager().SetTimer(RecoilTimaerManager_, this, &AGun::ResetRecoil, ResetRecoilTimer_, true);
	GetWorldTimerManager().PauseTimer(RecoilTimaerManager_);
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ������ �ѱ�ݵ� ���� ���� ����
	AController* tmpController = GetOwnerController();
	
	if (!tmpController) return;

	FRotator CurrentRotation = tmpController->GetControlRotation();
	FRotator TickRecoilRotation_ = CurrentRotation + ResultRecoilRotation_;

	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TickRecoilRotation_, DeltaTime, RecoilSpeed_);

	//UE_LOG(LogTemp, Warning, TEXT("CurrentRotation Pitch >> %f, Yaw >>  %f"), CurrentRotation.Pitch, CurrentRotation.Yaw);
	//UE_LOG(LogTemp, Warning, TEXT("NewRotation     Pitch >> %f, Yaw >>  %f"), NewRotation.Pitch, NewRotation.Yaw);
	//UE_LOG(LogTemp, Warning, TEXT("TargetRecoil    Pitch >> %f, Yaw >>  %f"), TargetRecoilRotation_.Pitch, TargetRecoilRotation_.Yaw);

	tmpController->SetControlRotation(NewRotation);
}

bool AGun::GunTrace(FHitResult& _HitResult, FVector& _ShotDir)
{	
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return false;

	// BulletTrace�� ������
	FVector ActorLocation = GetActorLocation();
	FRotator ActorRotator = GetActorRotation();

	UE_LOG(LogTemp, Warning, TEXT("ActorRotator: Pitch: %f, Yaw: %f, Roll: %f"), ActorRotator.Pitch, ActorRotator.Yaw, ActorRotator.Roll);

	OwnerController->GetPlayerViewPoint(ActorLocation, ActorRotator);

	// BulletTrace�� ����
	FVector End = ActorLocation + ActorRotator.Vector() * MaxRange_;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	FVector ShotDir = -ActorRotator.Vector();

	// ����Ʈ���̽��� ���
	return GetWorld()->LineTraceSingleByChannel(_HitResult, ActorLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	// ��Ʈ�ѷ��� ������� Pawn���� ĳ����
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)  return nullptr;

	return OwnerPawn->GetController();
}

void AGun::AddRecoil()
{
	ResultRecoilRotation_.Pitch += FMath::RandRange(-PitchRecoilIntensity_, PitchRecoilIntensity_);
	ResultRecoilRotation_.Yaw += FMath::RandRange(-YawRecoilIntensity_, YawRecoilIntensity_);
	UE_LOG(LogTemp, Warning, TEXT("ResultRecoilRotation_ Pitch >> %f, Yaw >>  %f"), ResultRecoilRotation_.Pitch, ResultRecoilRotation_.Yaw);

	GetWorldTimerManager().UnPauseTimer(RecoilTimaerManager_);
}

// �ݵ��� ����
void AGun::ResetRecoil()
{
	ResultRecoilRotation_ = FRotator::ZeroRotator;
	GetWorldTimerManager().PauseTimer(RecoilTimaerManager_);
}

void AGun::Reload()
{
	AShooterCharacter* Player = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (Player == false) return;
	
	Player->PlayReloadAnim();
}
