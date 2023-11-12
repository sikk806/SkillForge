// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Sword.generated.h"

/**
 * 
 */
UCLASS()
class SKILLFORGE_API ASword : public AWeapon
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ASword();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon | Sword")
	EWeaponState SwordState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Particle")
	bool bSwordParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Sound")
	class USoundCue* OnEquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Sound")
	USoundCue* SwingSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="SkeletalMesh")
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon | Combat")
	class UBoxComponent* CombatCollision;

	// PickUP State
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	// Equipped State
	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ActivateCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();
	
	void Equip(class AMain* Char);
	
	FORCEINLINE void SetWeaponState(EWeaponState State) { SwordState = State; }
	FORCEINLINE EWeaponState GetWeaponState() { return SwordState; }
};
