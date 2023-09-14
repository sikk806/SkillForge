// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LichLaser.generated.h"

UCLASS()
class SKILLFORGE_API ALichLaser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALichLaser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skills")
	class UCapsuleComponent* Laser;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills | Particle")
	UParticleSystemComponent* LaserParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills | Warning")
	class UStaticMeshComponent* LaserWarning;

	bool bAttack;



	UFUNCTION()
	virtual void LaserOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


};