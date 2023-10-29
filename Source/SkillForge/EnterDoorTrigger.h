// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnterDoorTrigger.generated.h"

UCLASS()
class SKILLFORGE_API AEnterDoorTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnterDoorTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skills")
	TSubclassOf<class AEnemy> Lich;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CutScene")
	class ULevelSequence* LevelSequence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CutScene")
	class ALevelSequenceActor* LevelSequenceActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door1")
	class UStaticMeshComponent* Door1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door2")
	class UStaticMeshComponent* Door2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void LichAppearance();

	bool bEnterCheck;
	float CheckTime;

	FRotator DoorRotation1;
	FRotator DoorRotation2;

};
