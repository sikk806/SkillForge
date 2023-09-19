// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LichWind.generated.h"

UCLASS()
class SKILLFORGE_API ALichWind : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALichWind();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	class UCapsuleComponent* Wind;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills | Particle")
	UParticleSystemComponent* WindParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills | Warning")
	bool bAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	int WindNo;

	FVector InitialLocation;
	float CurrentAngle;
	bool CheckWind;
	bool WindCycle;


};
