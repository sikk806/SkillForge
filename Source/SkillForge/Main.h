// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_IdleWalk UMETA(DisplayName = "IdleWalk"),
	EMS_Run UMETA(DisplayName = "Run"),
	EMS_RollStart UMETA(DisplayName = "RollStart"),
	EMS_Roll UMETA(DisplayName = "Roll"),
};

UENUM(BlueprintType)
enum class EIdentityStatus : uint8
{
	EIS_Normal UMETA(DisplayName = "Normal"),
	EIS_SteminaDecrease UMETA(DisplayName = "SteminaDecrease"),
	EIS_BelowMinimum UMETA(DisplayName = "BelowMinimum"),
	EIS_Exhausted UMETA(DisplayName = "Exhausted"),
	EIS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering"),
	EIS_Rolling UMETA(DisplayName = "Rolling"),

	EIS_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class SKILLFORGE_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

	// Equipped Setting
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	class ASword* EquippedSword;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class AMainPlayerController* MainPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	FVector CombatTargetLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UInputComponent* MyInputComponent;

	// Get Camera Setting
	FORCEINLINE class USpringArmComponent* GetSpringArm() { return SpringArm; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() { return FollowCamera; }

	// Equipped Setting
	FORCEINLINE void SetEquippedWeapon(ASword* WeaponToSet) {EquippedSword = WeaponToSet;}

	void ZoomCamera(float Value);

	// Use In MainAnimInstance
	float ForwardValue;
	float RightValue;

	// Use In RollStart
	float FV; // Save For ForwardValue During Rolling
	float RV; // Save For RightValue During Rolling

	//ENUMS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum | Movement")
	EMovementStatus MovementStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum | Identity")
	EIdentityStatus IdentityStatus; 

	FORCEINLINE void SetIdentityStatus(EIdentityStatus IStatus) { IdentityStatus = IStatus; }

	// Status
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxIdentity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Identity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float DrainMPRate;

	// Status - Death
	void DecrementHealth(float Amount);
	void IncrementHealth(float Amount);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;
	void Die();

	// Identity Info
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	float IdentityDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	float MinIdentity;

	// Movement Function & Parameter (MFP)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	// Movement Setting
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Speed")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Speed")
	float WalkBackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Speed")
	float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Speed")
	float RollSpeed;

	// MFP - Walk
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUpRate(float rate);
	void TurnRate(float rate);
	// MFP - Attack
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement | Attack")
	bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement | Attack")
	bool bHasCombatTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class AEnemy* CombatTargets;

	FORCEINLINE void SetCombatTarget(AEnemy* Target) { CombatTargets = Target; }
	FORCEINLINE void SetHasCombatTarget(bool HasTarget) { bHasCombatTarget = HasTarget; };

	bool bLMBDown;
	bool bRMBDown;
	FName ContinAttack;
	void LMBDown();
	void LMBUp();
	void RMBDown();
	void RMBUp();
	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Attack")
	class UAnimMontage* CombatMontage;

	// MFP - Identity_1
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Identity")
	bool bShiftKeyDown;

	void ShiftKeyDown();
	void ShiftKeyUp();

	// MFP - Identity_2
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Identity")
	bool bSpaceKeyDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Identity")
	bool bRolling;

	FTimerHandle RollTimerHandle;

	void SpaceKeyDown();
	void SpaceKeyUp();
	void StartRollState();
	void EndRollState(EMovementStatus Status);

	void SetMovementStatus(EMovementStatus Status);

	// Skills
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Attack")
	class UAnimMontage* SkillMontage;

	void QSkill();
	void ESkill();
	void RSkill();

	UFUNCTION(BlueprintCallable)
	void StingSkill();

	UFUNCTION(BlueprintCallable)
	void SwordFallSkillBegin();

	UFUNCTION(BlueprintCallable)
	void SwordFallSkill();

	UFUNCTION(BlueprintCallable)
	void Buff();

	UFUNCTION(BlueprintCallable)
	void BuffOff();

	FTimerHandle BuffTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skills")
	TSubclassOf<class AMainSting> Sting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skills")
	TSubclassOf<class AMainSwordFalling> SwordFallBegin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skills")
	TSubclassOf<class AMainSwordFalling> SwordFall;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skills")
	TSubclassOf<class AMainBuff> ActiveBuff;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills")
	UParticleSystemComponent* SwordBuff;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills")
	float BuffDecrementDamage;

	// Interp When Attacking Enemy
	float InterpSpeed;
	bool bInterpToEnemy;
	void SetInterpToEnemy(bool Interp);

	// cutScene
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CutScene")
	bool bCutScene;

	float SceneTime;


	// ect.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
	class UParticleSystem* HitParticles;

	UAnimInstance *AnimStopMontage;
	TArray<UAnimMontage*> ActiveMontages;

	UFUNCTION(BlueprintCallable)
	float GetCombatTargetHealth();

	UFUNCTION(BlueprintCallable)
	float GetCombatTargetMaxHealth();

};
