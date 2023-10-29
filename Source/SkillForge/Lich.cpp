// Fill out your copyright notice in the Description page of Project Settings.

#include "Main.h"
#include "Lich.h"
#include "LichLaser.h"
#include "LichSkull.h"
#include "LichWind.h"
#include "LichWave.h"
#include "EnterDoorTrigger.h"

#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieScene.h"

ALich::ALich()
{
	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLightComponent->SetupAttachment(GetRootComponent());

	AppearanceParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AppearanceParticle"));
	AppearanceParticle->SetupAttachment(GetRootComponent());

	bAttack = false;
	bDoSkill = false;
	bIsCombatOverlapping = false;
	bSwordPattern = false;
	bDoingPattern = false;
	bAppearance = true;

	MaxHealth = 1000;
	Health = 1000;

	WaveCnt = 0;

	DeadLichValue = 2.f;
}

void ALich::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	EnemyMovementState = EEnemyMovementState::EMS_Idle;

	AppearanceParticle->Deactivate();

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &ALich::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &ALich::AgroSphereOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &ALich::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &ALich::CombatSphereOnOverlapEnd);

	Appearance();
}

void ALich::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Health < MaxHealth && !bSwordPattern && !bDoingPattern && !bAppearance)
	{
		bSwordPattern = true;
		bDoingPattern = true;

		GetWorldTimerManager().SetTimer(PatternTime, this, &ALich::LichSword, 0.5f);
	}

	if (Alive() && !bDoingPattern && !bAppearance)
	{
		if (bIsCombatOverlapping)
		{
			if (CombatTarget && !bDoSkill)
			{
				GetCharacterMovement()->MaxWalkSpeed = 0.f;
				bAttack = true;
				SetEnemyMovementState(EEnemyMovementState::EMS_Attack);

				UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
				if (AnimInstance && CombatMontage)
				{
					int32 SkillNum = FMath::RandRange(0, 3);
					switch (SkillNum)
					{
					case 0:
						AnimInstance->Montage_Play(CombatMontage, 1.0f);
						AnimInstance->Montage_JumpToSection(FName("Spell"), CombatMontage);
						if (Laser)
						{
							SetDoSkill(true);
							FVector StartLocation = GetActorLocation();
							FRotator ActorRotation = GetActorRotation();

							FVector ForwardVector = ActorRotation.Vector().GetSafeNormal();
							ALichLaser *LichLaser = GetWorld()->SpawnActor<ALichLaser>(Laser, StartLocation + ForwardVector * 700.f, GetActorRotation());
							if (LichLaser)
							{
								LichLaser->bAttack = true;
							}
						}
						break;
					case 1:
						AnimInstance->Montage_Play(CombatMontage, 1.0f);
						AnimInstance->Montage_JumpToSection(FName("AttackRight"), CombatMontage);
						if (Wave)
						{
							SetDoSkill(true);
							WaveCnt = 0;
							FourWave();
						}
						break;
					case 2:
						AnimInstance->Montage_Play(CombatMontage, 1.0f);
						AnimInstance->Montage_JumpToSection(FName("Call"), CombatMontage);
						if (Skull)
						{
							SetDoSkill(true);
							for (int i = 0; i < 20; i++)
							{
								FVector MinRange = FVector(-10.0f, -10.0f, 0.0f);
								FVector MaxRange = FVector(10.0f, 10.0f, 0.0f);

								FVector RandomLocation = FMath::RandPointInBox(FBox(MinRange, MaxRange));

								FVector StartLocation = GetActorLocation();
								FRotator ActorRotation = GetActorRotation();

								FVector ForwardVector = ActorRotation.Vector().GetSafeNormal();
								StartLocation.Z += 75.f;
								StartLocation += (RandomLocation * 100.f);
								ALichSkull *LichSkull = GetWorld()->SpawnActor<ALichSkull>(Skull, StartLocation + ForwardVector * 100.f, GetActorRotation());
								if (LichSkull)
								{
									LichSkull->bAttack = true;
								}
							}
						}
						break;
					}
				}
			}
		}
	}

	if (!Alive() && !bAppearance)
	{

		USkeletalMeshComponent *MeshComponent = FindComponentByClass<USkeletalMeshComponent>();
		if (MeshComponent)
		{
			if (MeshComponent->GetNumMaterials() > 0)
			{
				UMaterialInterface *Material = MeshComponent->GetMaterial(0);
				if (Material)
				{
					UMaterialInstanceDynamic *DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);
					if (!DynamicMaterial)
					{
						DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
					}

					if (DynamicMaterial)
					{
						DeadLichValue -= DeltaTime * 0.5f;
						DynamicMaterial->SetScalarParameterValue(TEXT("Dissolve"), DeadLichValue);

						MeshComponent->SetMaterial(0, DynamicMaterial);
					}
				}
			}
		}
	}
}

void ALich::AgroSphereOnOverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (Alive() && !bDoingPattern && !bAppearance)
	{
		Super::AgroSphereOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
		if (OtherActor)
		{
			AMain *Main = Cast<AMain>(OtherActor);
			if (Main)
			{
			}
		}
	}
}

void ALich::AgroSphereOnOverlapEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (Alive() && !bDoingPattern && !bAppearance)
	{
		Super::AgroSphereOnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	}
}

void ALich::CombatSphereOnOverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (Alive() && !bDoingPattern && !bAppearance)
	{
		Super::CombatSphereOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
		if (OtherActor)
		{
			AMain *Main = Cast<AMain>(OtherActor);
			if (Main)
			{
				bIsCombatOverlapping = true;
				GetCharacterMovement()->MaxWalkSpeed = 0.f;
				bAttack = true;
				SetEnemyMovementState(EEnemyMovementState::EMS_Attack);

				UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
				if (AnimInstance && CombatMontage && !bDoSkill)
				{
					int32 SkillNum = FMath::RandRange(0, 3);
					switch (SkillNum)
					{
					case 0:
						AnimInstance->Montage_Play(CombatMontage, 1.0f);
						AnimInstance->Montage_JumpToSection(FName("Spell"), CombatMontage);
						if (Laser)
						{
							SetDoSkill(true);
							FVector StartLocation = GetActorLocation();
							FRotator ActorRotation = GetActorRotation();

							FVector ForwardVector = ActorRotation.Vector().GetSafeNormal();
							ALichLaser *LichLaser = GetWorld()->SpawnActor<ALichLaser>(Laser, StartLocation + ForwardVector * 700.f, GetActorRotation());
							if (LichLaser)
							{
								LichLaser->bAttack = true;
							}
						}
						break;
					case 1:
						AnimInstance->Montage_Play(CombatMontage, 1.0f);
						AnimInstance->Montage_JumpToSection(FName("AttackRight"), CombatMontage);
						if (Wave)
						{
							SetDoSkill(true);
							WaveCnt = 0;
							FourWave();
						}
						break;
					case 2:
						AnimInstance->Montage_Play(CombatMontage, 1.0f);
						AnimInstance->Montage_JumpToSection(FName("Call"), CombatMontage);
						if (Skull)
						{
							SetDoSkill(true);
							for (int i = 0; i < 20; i++)
							{
								FVector MinRange = FVector(-10.0f, -10.0f, 0.0f);
								FVector MaxRange = FVector(10.0f, 10.0f, 0.0f);

								FVector RandomLocation = FMath::RandPointInBox(FBox(MinRange, MaxRange));

								FVector StartLocation = GetActorLocation();
								FRotator ActorRotation = GetActorRotation();

								FVector ForwardVector = ActorRotation.Vector().GetSafeNormal();
								StartLocation.Z += 75.f;
								StartLocation += (RandomLocation * 100.f);
								ALichSkull *LichSkull = GetWorld()->SpawnActor<ALichSkull>(Skull, StartLocation + ForwardVector * 100.f, GetActorRotation());
								if (LichSkull)
								{
									LichSkull->bAttack = true;
								}
							}
						}
						break;
					}
					// if (Wind)
					// {
					// 	for (int i = 0; i < 4; i++)
					// 	{
					// 		FVector StartLocation = GetActorLocation();
					// 		FRotator ActorRotation = GetActorRotation();

					// 		ActorRotation.Yaw += 90*i;

					// 		FVector ForwardVector = ActorRotation.Vector().GetSafeNormal();
					// 		ALichWind *LichWind = GetWorld()->SpawnActor<ALichWind>(Wind, StartLocation + ForwardVector * 150.f, ActorRotation);
					// 		if (LichWind)
					// 		{
					// 			LichWind->WindNo = i;
					// 			LichWind->bAttack = true;
					// 		}
					// 	}
					// }
				}
			}
		}
	}
}

void ALich::CombatSphereOnOverlapEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (Alive() && !bDoingPattern && !bAppearance)
	{
		Super::CombatSphereOnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
		if (OtherActor)
		{
			AMain *Main = Cast<AMain>(OtherActor);
			if (Main)
			{
				SetDoSkill(false);
				bIsCombatOverlapping = false;
			}
		}
	}
}

void ALich::Attack()
{
	SetDoSkill(false);
	if (bOverlappingCombatSphere)
	{
		AMain *Main = Cast<AMain>(CombatTarget);
		if (Main)
		{
			MoveToTarget(Main);
		}
	}
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void ALich::AttackEnd()
{
	bAttack = false;
	SetEnemyMovementState(EEnemyMovementState::EMS_Idle);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	if (bOverlappingCombatSphere)
	{
		// SetDoSkill(false);
		float AttackTime = FMath::FRandRange(1.5f, 2.5f);
		// SetTimer(FTimerHandle, this, function, Time(float))

		GetWorldTimerManager().SetTimer(AttackTimer, this, &ALich::Attack, AttackTime);
	}
}

void ALich::FourWave()
{
	if (WaveCnt < 5)
	{
		FVector StartLocation = GetActorLocation();
		FRotator ActorRotation = GetActorRotation();
		StartLocation.Z += 50.f;

		ActorRotation.Yaw += 90.f;
		StartLocation += (ActorRotation.Vector() * WaveCnt * 200.f);
		ALichWave *LichWave = GetWorld()->SpawnActor<ALichWave>(Wave, StartLocation, GetActorRotation());
		if (LichWave)
		{
			LichWave->bAttack = true;
		}
		GetWorldTimerManager().SetTimer(WaveTime, this, &ALich::FourWave, 0.25f);
		WaveCnt++;
	}
}

void ALich::LichSword()
{
}

void ALich::CutScene()
{
	if (!bDoingPattern)
	{
	}
}

void ALich::Appearance()
{
	FRotator Rot = GetActorRotation();
	Rot.Yaw += 180.f;
	SetActorRotation(Rot);
	AppearanceParticle->Activate();
	UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AppearanceMontage && bAppearance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Check"));
		AnimInstance->Montage_Play(AppearanceMontage, 0.5f);
		AnimInstance->Montage_JumpToSection(FName("Appearance"), AppearanceMontage);
	}
	else if(AnimInstance && AppearanceMontage && !bAppearance)
	{
		AnimInstance->Montage_Play(AppearanceMontage, -0.5f);
		AnimInstance->Montage_JumpToSection(FName("DisAppearance"), AppearanceMontage);
	}

	FTimerHandle AppearanceTime;
	GetWorldTimerManager().SetTimer(AppearanceTime, this, &ALich::SetBoolAppearance, 2.f);
}

void ALich::SetBoolAppearance()
{
	if(bAppearance) bAppearance = false;
	else if(!bAppearance) bAppearance = true;
}