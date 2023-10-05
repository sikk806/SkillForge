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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	class UBoxComponent* SwordFall;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills | Particle")
	UParticleSystemComponent* SwordFallBeginParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills | Particle")
	UParticleSystemComponent* SwordFallParticle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DestroySelf();
	FTimerHandle SwordFallTimer;

};
