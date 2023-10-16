// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainSwordFalling.generated.h"

UCLASS()
class SKILLFORGE_API AMainSwordFalling : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainSwordFalling();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	class AAIController* AIController;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	TSubclassOf<UDamageType> DamageTypeClass;

	TSet<AActor*> HitTargetsSet;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	class UBoxComponent* SwordFall;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills | Particle")
	UParticleSystemComponent* SwordFallBeginParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills | Particle")
	UParticleSystemComponent* SwordFallParticle;

	bool bAttack;
	bool bCombatOverlap;
	int cntHit;
	float SFTimer;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DestroySelf();
	void AttackingFalse();
	FTimerHandle SwordFallTimer;
	FTimerHandle AttackMoveTimer;

	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
