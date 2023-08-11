// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Lich.generated.h"

/**
 * 
 */
UCLASS()
class SKILLFORGE_API ALich : public AEnemy
{
	GENERATED_BODY()

public:
	ALich();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Status
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Custom")
	class UPointLightComponent* PointLightComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
	class UCapsuleComponent* Laser;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills | Particle")
	UParticleSystemComponent* LaserParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills | Warning")
	class UStaticMeshComponent* LaserWarning;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skills | Anim")
	class UAnimMontage* CombatMontage;

	bool bAttack;

	UFUNCTION()
	void LaserOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
};
