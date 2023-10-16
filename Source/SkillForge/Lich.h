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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skills")
	TSubclassOf<class ALichLaser> Laser;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skills")
	TSubclassOf<class ALichSkull> Skull;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skills")
	TSubclassOf<class ALichWind> Wind;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skills")
	TSubclassOf<class ALichWave> Wave;

	float DeadLichValue;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Status
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Custom")
	class UPointLightComponent* PointLightComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attack")
	bool bAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attack")
	bool bDoSkill;
	FORCEINLINE void SetDoSkill(bool DoSkill) { bDoSkill = DoSkill; };

	bool bIsCombatOverlapping;

	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	
	virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	
	virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	FTimerHandle WaveTime;
	void FourWave();
	int WaveCnt;



};
