// Fill out your copyright notice in the Description page of Project Settings.

#include "Main.h"
#include "Enemy.h"
#include "MainAnimInstance.h"
#include "MainPlayerController.h"
#include "MainSting.h"
#include "MainSwordFalling.h"
#include "MainBuff.h"
#include "Sword.h"
#include "Weapon.h"

#include "Animation/AnimInstance.h"
#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/EngineTypes.h"

// Sets default values
AMain::AMain()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 1000.f;
	SpringArm->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	SwordBuff = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("StingParticle"));
	SwordBuff->SetupAttachment(GetRootComponent());

	// Keep Forward Vector and Right Vector
	FV = 0;
	RV = 0;

	// Status
	MaxHealth = 100;
	Health = 100;

	MaxIdentity = 100;
	MinIdentity = 0;
	Identity = 100;
	IdentityDrainRate = 10;

	MaxMP = 100;
	MP = 100;
	DrainMPRate = 5;

	// Camera Setting
	BaseLookUpRate = 55.f;
	BaseTurnRate = 55.f;

	// Speed Idle, Run, Roll
	WalkSpeed = 300.f;
	RunSpeed = 450.f;
	RollSpeed = 600.f;

	// 0.0f ~ 1.0f
	BuffDecrementDamage = 1.0f;

	// For Right Attack
	ContinAttack = FName("AttackA2");
	bLMBDown = false;
	bRMBDown = false;
	bAttacking = false;
	bHasCombatTarget = false;

	InterpSpeed = 15.f;
	bInterpToEnemy = false;

	SwordBuff = nullptr;

	bCutScene = false;
	SceneTime = 0.f;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();

	MainPlayerController = Cast<AMainPlayerController>(GetController());

	// Camera Start Rotation
	FRotator SetCameraRotation = GetControlRotation();
	SetCameraRotation.Pitch = 335.f;

	Controller->SetControlRotation(SetCameraRotation);

	// Movement Init
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	SetMovementStatus(EMovementStatus::EMS_IdleWalk);

	// For Stop Montage (There are many similar declarations in other functions, so they will need to be modified later when optimizing.)
	AnimStopMontage = GetMesh()->GetAnimInstance();
	ActiveMontages.Add(CombatMontage);
	ActiveMontages.Add(SkillMontage);
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bCutScene && SceneTime < 2.f)
	{
		SceneTime += DeltaTime;
		MoveForward(1.f);
	}

	/* 1. Warrior Iden : Roll
	Roll Can Avoid Monster's Attack. But using the roll reduces the identity of 30.
	This Identity can't use under Identity 30.
	*/
	float DrainMP = DeltaTime * DrainMPRate;
	if(MP + DrainMP >= MaxMP)
	{
		MP = MaxMP;
	}
	else
	{
		MP += DrainMP;
	}

	if (MovementStatus == EMovementStatus::EMS_RollStart)
	{
		if (Identity >= 30.f)
		{
			if (AnimStopMontage)
			{
				for (UAnimMontage* MontageInstance : ActiveMontages)
				{
					AnimStopMontage->Montage_Stop(0.2f, MontageInstance);
				}
			}
			bAttacking = false;
			Identity -= 30.f;
			SetMovementStatus(EMovementStatus::EMS_Roll);
			FV = ForwardValue;
			RV = RightValue;
			if (FV == 0 && RV == 0)
			{
				FV = 1;
				RV = 0;
			}
		}
		else
		{
			bSpaceKeyDown = false;
			bRolling = false;
		}
	}

	if (MovementStatus == EMovementStatus::EMS_Roll)
	{
		bSpaceKeyDown = true;
		MoveForward(FV);
		MoveRight(RV);
	}
	else
	{
		// Using Warrior Iden: Move Faster.
		/* 2. Warrior Iden : Sprint
		|Exhausted : 0|MinIdentity -----|Identity--------------------------|
		2-1. 1/4 Status - Normal
		2-1-1. When bShiftKeyDown : Can run until Identity - DeltaIdentity <= MinIdentity and becomes EMS_run.
			   If Identity becomes MinIdentity Character still can Run and becomes EIS_BelowMinimum.
		2-1-2. When bShiftKeyUp : Just Recovering Identity until Identity + DeltaIdentity >= MaxIdentity and becomes EMS_IdelWalk.
		2-2. 2/4 Status - BelowMinimum
		2-2-1. When bShiftKeyDown : Can run until becomes '0'.
			   If Identity becomes '0' then Character becomes EMS_IdleWalk and EIS_Exhausted.
		2-2-2. When bShiftKeyUp : Just Recovering Identity until Identity + DeltaIdentity >= MinIdentity and becomes EMS_IdleWalk.
			   and becomes EMS_IdelWalk and EIS_Normal.
		2-3. 3/4 Status - Exhausted
		2-3-1. When bShiftKeyDown : Exhausted becomes when Identity is 0 with ShiftKeyDown.
			   Can't run anymore and becomes EMS_IdleWalk until Identity recovers to MinIdentity.
		2-3-2. When bShiftKeyUp : Becomes EIS_ExhaustedRecovering.
		2-4. 4/4 Status - ExhaustedRecovering
		2-4-1. Shift is not working. Recovering Identity until Identity + DeltaIdentity >= MinIdentity and becomes EIS_Normal and EMS_IdleWalk.
		*/
		float DeltaIdentity = IdentityDrainRate * DeltaTime;
		switch (IdentityStatus)
		{
		case EIdentityStatus::EIS_Normal:
			if (bShiftKeyDown)
			{
				if (Identity - DeltaIdentity <= MinIdentity)
				{
					SetIdentityStatus(EIdentityStatus::EIS_BelowMinimum);
					Identity -= DeltaIdentity;
				}
				else
				{
					Identity -= DeltaIdentity;
				}
				SetMovementStatus(EMovementStatus::EMS_Run);
			}
			else
			{
				if (Identity + DeltaIdentity >= MaxIdentity)
				{
					Identity = MaxIdentity;
				}
				else
				{
					Identity += DeltaIdentity;
				}
				SetMovementStatus(EMovementStatus::EMS_IdleWalk);
			}
			break;
		case EIdentityStatus::EIS_BelowMinimum:
			if (bShiftKeyDown)
			{
				if (Identity - DeltaIdentity <= 0.f)
				{
					SetIdentityStatus(EIdentityStatus::EIS_Exhausted);
					Identity = 0.f;
					SetMovementStatus(EMovementStatus::EMS_IdleWalk);
				}
				else
				{
					Identity -= DeltaIdentity;
					SetMovementStatus(EMovementStatus::EMS_Run);
				}
			}
			else
			{
				if (Identity + DeltaIdentity >= MinIdentity)
				{
					SetIdentityStatus(EIdentityStatus::EIS_Normal);
					Identity += DeltaIdentity;
				}
				else
				{
					Identity += DeltaIdentity;
				}
				SetMovementStatus(EMovementStatus::EMS_IdleWalk);
			}
			break;
		case EIdentityStatus::EIS_Exhausted:
			if (bShiftKeyDown)
			{
				Identity = 0.f;
			}
			else
			{
				Identity += DeltaIdentity;
				SetIdentityStatus(EIdentityStatus::EIS_ExhaustedRecovering);
			}
			SetMovementStatus(EMovementStatus::EMS_IdleWalk);
			break;
		case EIdentityStatus::EIS_ExhaustedRecovering:
			if (Identity + DeltaIdentity >= MinIdentity)
			{
				Identity += DeltaIdentity;
				SetIdentityStatus(EIdentityStatus::EIS_Normal);
			}
			else
			{
				Identity += DeltaIdentity;
			}
			SetMovementStatus(EMovementStatus::EMS_IdleWalk);
			break;
		}
	}

	if (CombatTargets)
	{
		CombatTargetLocation = CombatTargets->GetActorLocation();
		if (MainPlayerController)
		{
			MainPlayerController->EnemyLocation = CombatTargetLocation;
		}
	}
}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	MyInputComponent = PlayerInputComponent;

	PlayerInputComponent->BindAction("Identity_1", IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction("Identity_1", IE_Released, this, &AMain::ShiftKeyUp);
	PlayerInputComponent->BindAction("Identity_2", IE_Pressed, this, &AMain::SpaceKeyDown);
	PlayerInputComponent->BindAction("Identity_2", IE_Released, this, &AMain::SpaceKeyUp);
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMain::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMain::LMBUp);
	PlayerInputComponent->BindAction("RMB", IE_Pressed, this, &AMain::RMBDown);
	PlayerInputComponent->BindAction("RMB", IE_Released, this, &AMain::RMBUp);

	// Skill
	PlayerInputComponent->BindAction("Skill1", IE_Pressed, this, &AMain::QSkill);
	PlayerInputComponent->BindAction("Skill2", IE_Pressed, this, &AMain::ESkill);
	PlayerInputComponent->BindAction("Skill3", IE_Pressed, this, &AMain::RSkill);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpRate);
	PlayerInputComponent->BindAxis("Zoom", this, &AMain::ZoomCamera);
}

void AMain::ZoomCamera(float Value)
{
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength + Value * 50, 600.f, 1000.f);
}

void AMain::MoveForward(float Value)
{
	ForwardValue = Value;
	if (Controller != nullptr && Value != 0.f && !bSpaceKeyDown && !bAttacking && !bCutScene)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
	else if (bSpaceKeyDown)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, FV);
	}
	else if(bCutScene)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void AMain::MoveRight(float Value)
{
	RightValue = Value;
	if (Controller != nullptr && Value != 0.f && !bSpaceKeyDown && !bAttacking  && !bCutScene)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
	else if (bSpaceKeyDown)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, RV);
	}
}

void AMain::LookUpRate(float rate)
{
	if (!bSpaceKeyDown && !bAttacking && !bCutScene)
	{
		float CameraPitch = GetControlRotation().Pitch;
		float NewCameraPitch = CameraPitch + rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds();
		float DeltaPitch = rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds();

		// 최소값과 최댓값을 지정
		float MinPitch = 325.f;
		float MaxPitch = 350.f;

		// NewCameraPitch 값을 MinPitch와 MaxPitch 사이로 동기화
		NewCameraPitch = FMath::Clamp(NewCameraPitch, MinPitch, MaxPitch);

		// SetControlRotation을 사용하여 Pitch 값을 설정
		FRotator NewControlRotation = GetControlRotation();
		NewControlRotation.Pitch = NewCameraPitch;
		Controller->SetControlRotation(NewControlRotation);
	}
}

void AMain::TurnRate(float rate)
{
	if (!bSpaceKeyDown && !bAttacking && !bCutScene)
	{
		AddControllerYawInput(rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AMain::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_IdleWalk)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	else if (MovementStatus == EMovementStatus::EMS_Run)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else if (MovementStatus == EMovementStatus::EMS_Roll)
	{
		GetCharacterMovement()->MaxWalkSpeed = RollSpeed;
	}
}

void AMain::ShiftKeyDown()
{
	if (!bSpaceKeyDown)
	{
		bShiftKeyDown = true;
		// SetMovementStatus(EMovementStatus::EMS_Run);
	}
}

void AMain::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

void AMain::SpaceKeyDown()
{
	if (!bSpaceKeyDown)
	{
		EMovementStatus Status;
		Status = MovementStatus;
		bSpaceKeyDown = true;
		SetMovementStatus(EMovementStatus::EMS_RollStart);

		// Keep EMS After Rolling.
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this, Status]()
								 { EndRollState(Status); });

		GetWorldTimerManager().SetTimer(RollTimerHandle, TimerDelegate, 1.0f, false);
	}
}

void AMain::SpaceKeyUp()
{
	// bSpaceKeyDown = false;
}

void AMain::StartRollState()
{
}

// Set EMS after Rolling.
void AMain::EndRollState(EMovementStatus Status)
{
	if (MovementStatus == EMovementStatus::EMS_Roll)
	{
		bSpaceKeyDown = false;
		bRolling = false;
		FV = 0;
		RV = 0;
		SetMovementStatus(Status);
	}
}

void AMain::LMBDown()
{
	if (!bLMBDown && !bRMBDown && (MovementStatus != EMovementStatus::EMS_Roll))
	{
		bLMBDown = true;
		
		ContinAttack = FName("AttackA2");
		if(EquippedWeapon)
			EquippedWeapon->Damage = 10.f;
		Attack();
	}
}

void AMain::LMBUp()
{
	if (bLMBDown)
	{
		bLMBDown = false;
	}
}

void AMain::RMBDown()
{
	if (!bLMBDown && !bRMBDown && (MovementStatus != EMovementStatus::EMS_Roll))
	{
		bRMBDown = true;
		Attack();
	}
}

void AMain::RMBUp()
{
	if (bRMBDown)
	{
		bRMBDown = false;
		ContinAttack = FName("AttackA2");
		if(EquippedWeapon)
			EquippedWeapon->Damage = 10.f;
	}
}

void AMain::Attack()
{
	if (!bAttacking && EquippedWeapon)
	{
		bAttacking = true;
		SetInterpToEnemy(true);

		UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && CombatMontage)
		{
			if (bLMBDown)
			{
				EquippedWeapon->Damage = 10.f;
				AnimInstance->Montage_Play(CombatMontage, 2.f);
				AnimInstance->Montage_JumpToSection(FName("AttackA1"), CombatMontage);
			}
			else if (bRMBDown)
			{
				AnimInstance->Montage_Play(CombatMontage, 1.3f);
				AnimInstance->Montage_JumpToSection(ContinAttack, CombatMontage);
			}
		}
	}
}

void AMain::AttackEnd()
{
	bAttacking = false;
	if (bLMBDown || bRMBDown)
	{
		if (bRMBDown && EquippedWeapon)
		{
			if (ContinAttack == FName("AttackA2"))
			{
				ContinAttack = FName("AttackA3");
				EquippedWeapon->Damage = 15.f;
			}
			else if (ContinAttack == FName("AttackA3"))
			{
				ContinAttack = FName("AttackA4");
				EquippedWeapon->Damage = 20.f;
			}
			else if (ContinAttack == FName("AttackA4"))
			{
				ContinAttack = FName("AttackA2");
				EquippedWeapon->Damage = 10.f;
			}
		}
		Attack();
	}
}

void AMain::SetInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}

void AMain::DecrementHealth(float Amount)
{
	if (Health - Amount <= 0)
	{
		Health -= Amount;
		Die();
	}
	else
	{
		Health -= Amount;
	}
}

void AMain::IncrementHealth(float Amount)
{
}

float AMain::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
	DecrementHealth(DamageAmount * BuffDecrementDamage);

	return DamageAmount;
}

void AMain::Die()
{
	UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.f);
		AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
	}
}

// SKills
void AMain::QSkill()
{
	if (EquippedWeapon && !bAttacking)
	{
		UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && SkillMontage)
		{
			if (MP < 33)
			{
				return;
			}
			bAttacking = true;
			MP -= 33;
			AnimInstance->Montage_Play(SkillMontage, 1.f);
			AnimInstance->Montage_JumpToSection(FName("Sting"), SkillMontage);
		}
	}
}

void AMain::ESkill()
{
	if (EquippedWeapon && !bAttacking)
	{
		UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && SkillMontage)
		{
			if (MP < 99)
			{
				return;
			}
			bAttacking = true;
			MP -= 99;
			AnimInstance->Montage_Play(SkillMontage, 1.f);
			AnimInstance->Montage_JumpToSection(FName("SwordFall"), SkillMontage);
		}
	}
}

void AMain::RSkill()
{
	if (EquippedWeapon && !bAttacking)
	{
		UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && SkillMontage)
		{
			if (MP < 66)
			{
				return;
			}
			bAttacking = true;
			MP -= 66;
			AnimInstance->Montage_Play(SkillMontage, 1.f);
			AnimInstance->Montage_JumpToSection(FName("Buff"), SkillMontage);
		}
	}
}

void AMain::StingSkill()
{
	FVector StartLocation = GetActorLocation();
	FRotator ActorRotation = GetActorRotation();

	FVector ForwardVector = ActorRotation.Vector().GetSafeNormal();
	AMainSting *MainSting = GetWorld()->SpawnActor<AMainSting>(Sting, StartLocation + ForwardVector * 200.f, GetActorRotation());
}

void AMain::SwordFallSkillBegin()
{
	FVector StartLocation = GetActorLocation();
	FRotator ActorRotation = GetActorRotation();


	FVector ForwardVector = ActorRotation.Vector().GetSafeNormal();
	AMainSwordFalling *MainSwordFalling = GetWorld()->SpawnActor<AMainSwordFalling>(SwordFallBegin, StartLocation, GetActorRotation());
	MainSwordFalling->SwordFallParticle->Deactivate();
}

void AMain::SwordFallSkill()
{
	FVector StartLocation = GetActorLocation();
	FRotator ActorRotation = GetActorRotation();

	FVector ForwardVector = ActorRotation.Vector().GetSafeNormal();
	AMainSwordFalling *MainSwordFalling = GetWorld()->SpawnActor<AMainSwordFalling>(SwordFall, StartLocation + ForwardVector * 500.f, GetActorRotation());
	MainSwordFalling->SwordFall->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MainSwordFalling->bAttack = true;
	MainSwordFalling->SwordFallBeginParticle->Deactivate();
}

void AMain::Buff()
{
	FVector StartLocation = GetActorLocation();
	FRotator ActorRotation = GetActorRotation();

	FVector ForwardVector = ActorRotation.Vector().GetSafeNormal();
	ForwardVector.Z -= 90.f;
	AMainBuff *MainBuff = GetWorld()->SpawnActor<AMainBuff>(ActiveBuff, StartLocation + ForwardVector, GetActorRotation());
	SwordBuff->SetTemplate(MainBuff->ShieldParticle->Template);

	GetWorldTimerManager().SetTimer(BuffTimer, this, &AMain::BuffOff, 9.5f);
}

void AMain::BuffOff()
{
	GetWorldTimerManager().ClearTimer(BuffTimer);
	BuffDecrementDamage = 1.0f;
}

float AMain::GetCombatTargetHealth()
{
    if (CombatTargets)
    {
        return CombatTargets->Health;
    }
    else
    {
        return 0;
    }
}

float AMain::GetCombatTargetMaxHealth()
{
	if (CombatTargets)
    {
        return CombatTargets->MaxHealth;
    }
    else
    {
        return 0;
    }
}