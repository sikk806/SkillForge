// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainBuff.generated.h"

UCLASS()
class SKILLFORGE_API AMainBuff : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainBuff();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	class USphereComponent* Buff;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills | Particle")
	UParticleSystemComponent* BuffParticle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills | Particle")
	UParticleSystemComponent* ShieldParticle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DestroySelf();
	FTimerHandle BuffTimer;

};
