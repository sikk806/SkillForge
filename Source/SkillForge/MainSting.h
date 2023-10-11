// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainSting.generated.h"

UCLASS()
class SKILLFORGE_API AMainSting : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainSting();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	class AAIController* AIController;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	TSubclassOf<UDamageType> DamageTypeClass;

	TSet<AActor*> HitTargetsSet;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	class UCapsuleComponent* Sting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills | Particle")
	UParticleSystemComponent* StingParticle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void DestroySelf();
	FTimerHandle StingTimer;

	int32 StingTime;
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
