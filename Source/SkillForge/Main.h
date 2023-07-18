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
	EMS_Roll UMETA(DisplayName = "Roll"),
};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_SteminaDecrease UMETA(DisplayName = "SteminaDecrease"),
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering"),
	ESS_Rolling UMETA(DisplayName = "Rolling"),

	ESS_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class SKILLFORGE_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Get Camera Setting
	FORCEINLINE class USpringArmComponent* GetSpringArm() { return SpringArm; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() { return FollowCamera; }

	// Use In MainAnimInstance
	float ForwardValue;
	float RightValue;

	// Movement Function & Parameter
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Enum")
	EMovementStatus MovementStatus;

	// MFP - Walk
	void MoveForward(float Value);

	void MoveRight(float Value);

	void LookUpRate(float rate);

	void TurnRate(float rate);

	// MFP - Identity_1
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Identity")
	bool bShiftKeyDown;

	void ShiftKeyDown();
	void ShiftKeyUp();

	// MFP - Identity_2
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Identity")
	bool bSpaceKeyDown;

	void SpaceKeyDown();
	void SpaceKeyUp();

	void SetMovementStatus(EMovementStatus Status);


};
