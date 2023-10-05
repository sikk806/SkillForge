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

};
