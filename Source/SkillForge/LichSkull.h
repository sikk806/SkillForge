// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LichSkull.generated.h"

UCLASS()
class SKILLFORGE_API ALichSkull : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALichSkull();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills")
	class USphereComponent* Skull;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills | Particle")
	class UNiagaraComponent* SkullMuzzle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills | Particle")
	class UNiagaraComponent* SkullHit;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills | Warning")
	class UStaticMeshComponent* SkullWarning;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills | Warning")
	bool bAttack;

	bool bSkullOn;
	bool bSkullBoom;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void DestroySkull();
	void SkullOff();

	FTimerHandle SkullTimer;

};
