// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Main.h"
#include "AIController.h"
#include "MainPlayerController.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/DamageType.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());

	bOverlappingCombatSphere = false;

	DeathDelay = 2.f;
	AlphaValue = 1.0f;
	CombatTarget = nullptr;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (Alive())
	{
		if (OtherActor)
		{
			AMain *Main = Cast<AMain>(OtherActor);
			if (Main)
			{
				MoveToTarget(Main);
			}
		}
	}
}

void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (Alive())
	{
		if (OtherActor)
		{
			AMain *Main = Cast<AMain>(OtherActor);
			if (Main)
			{
				if (Main->CombatTargets == this)
				{
					Main->SetCombatTarget(nullptr);
					Main->SetHasCombatTarget(false);
				}
				if (Main->MainPlayerController)
				{
					Main->MainPlayerController->RemoveEnemyHealthBar();
				}
				SetEnemyMovementState(EEnemyMovementState::EMS_Idle);
				if (AIController)
				{
					AIController->StopMovement();
				}
			}
		}
	}
}

void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		AMain *Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			Main->SetCombatTarget(this);
			Main->SetHasCombatTarget(true);
			if (Main->MainPlayerController)
			{
				Main->MainPlayerController->DisplayEnemyHealthBar();
			}
			CombatTarget = Main;
			bOverlappingCombatSphere = true;
		}
	}
}

void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (Alive())
	{
		if (OtherActor)
		{
			AMain *Main = Cast<AMain>(OtherActor);
			if (Main)
			{
				bOverlappingCombatSphere = false;
				if (EnemyMovementState != EEnemyMovementState::EMS_Attack && EnemyMovementState != EEnemyMovementState::EMS_Death)
				{
					MoveToTarget(Main);
					CombatTarget = nullptr;
				}
			}
		}
	}
}

void AEnemy::MoveToTarget(class AMain *Target)
{
	if (Alive() && EnemyMovementState != EEnemyMovementState::EMS_Death)
	{
		SetEnemyMovementState(EEnemyMovementState::EMS_MoveToTarget);
		if (AIController)
		{
			FAIMoveRequest MoveRequest;
			MoveRequest.SetGoalActor(Target);
			MoveRequest.SetAcceptanceRadius(25.f);

			FNavPathSharedPtr NavPath;

			AIController->MoveTo(MoveRequest, &NavPath);
		}
	}
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
	if (Health - DamageAmount <= 0.f)
	{
		Health -= DamageAmount;
		Die();
	}
	else
	{
		Health -= DamageAmount;
	}

	return DamageAmount;
}

void AEnemy::Die()
{
	SetEnemyMovementState(EEnemyMovementState::EMS_Death);
	UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		if (CombatMontage)
		{
			AnimInstance->Montage_Play(CombatMontage, 0.5f);
			AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
		}
	}
	bAttacking = false;
	if (CombatCollision)
		CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (AgroSphere)
		AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (CombatSphere)
		CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AMain *Main = Cast<AMain>(CombatTarget);
	if (Main)
	{
		Main->bHasCombatTarget = false;
		if (Main->MainPlayerController)
		{
			Main->MainPlayerController->RemoveEnemyHealthBar();
		}
	}
}

void AEnemy::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear, DeathDelay);
}

bool AEnemy::Alive()
{
	return GetEnemyMovementState() != EEnemyMovementState::EMS_Death;
}

void AEnemy::Disappear()
{
	Destroy();
}