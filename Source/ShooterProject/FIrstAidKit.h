// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FIrstAidKit.generated.h"

UCLASS()
class SHOOTERPROJECT_API AFIrstAidKit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFIrstAidKit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh_;

	UPROPERTY(EditAnywhere)
	float AddHealth_ = 30.f;

	UPROPERTY(EditAnywhere)
	float SpinSpeed_ = 10.f;

	UFUNCTION()
	void PlayerOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
