// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LichSwordPattern.generated.h"

UCLASS()
class SKILLFORGE_API ALichSwordPattern : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALichSwordPattern();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	class UCapsuleComponent* BlackSwordHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	class UCapsuleComponent* WhiteSwordHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	class USphereComponent* PoisonBombHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	class UStaticMeshComponent* BlackSword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	class UStaticMeshComponent* WhiteSword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	class UParticleSystemComponent* PoisonBomb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	class UParticleSystemComponent* BlackSafeZone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	class UParticleSystemComponent* WhiteSafeZone;

	int WhiteSwordHitCnt;
	int BlackSwordHitCnt;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnBlackSwordOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnWhiteSwordOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};
