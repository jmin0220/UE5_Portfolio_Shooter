// Fill out your copyright notice in the Description page of Project Settings.


#include "FIrstAidKit.h"
#include "ShooterCharacter.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AFIrstAidKit::AFIrstAidKit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh_ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FirstAidKit"));
	StaticMesh_->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFIrstAidKit::BeginPlay()
{
	Super::BeginPlay();
	
	StaticMesh_->OnComponentBeginOverlap.AddDynamic(this, &AFIrstAidKit::PlayerOverlapEvent);
}

// Called every frame
void AFIrstAidKit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 회전
	FRotator actorRot = GetActorRotation();
	actorRot.Yaw += SpinSpeed_ * DeltaTime;
	SetActorRotation(actorRot);
}

void AFIrstAidKit::PlayerOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어가 아닌 액터와 오버랩될경우 무시
	if (OtherActor != UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		return;
	}

	AShooterCharacter* tmpPlayer_ = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	// 체력이 100%가 아닌 경우에만
	if (tmpPlayer_->GetHealthPercent() < 1.f)
	{
		tmpPlayer_->AddHealth(AddHealth_);

		Destroy();
	}
}

