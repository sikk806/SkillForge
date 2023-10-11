// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LichWave.generated.h"

UCLASS()
class SKILLFORGE_API ALichWave : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALichWave();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	class AAIController* AIController;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	TSubclassOf<UDamageType> DamageTypeClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class ALich* Lich;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	class UCapsuleComponent* Wave;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills | Particle")
	UParticleSystemComponent* WaveParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills | Warning")
	class UStaticMeshComponent* WaveWarning;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills | Warning")
	bool bAttack;
	
	bool bWaveOn;
	bool bMove;
	float nowTime;

	UFUNCTION()
	void DestroyWave();
	void WaveOff(float DeltaTime);
	void MoveAttack();

	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	FTimerHandle WaveTimer;
	FTimerHandle HitWaveTimer;
};
