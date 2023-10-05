// Fill out your copyright notice in the Description page of Project Settings.

#include "Main.h"
#include "Lich.h"
#include "LichLaser.h"
#include "LichSkull.h"
#include "LichWind.h"
#include "LichWave.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

ALich::ALich()
{
	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLightComponent->SetupAttachment(GetRootComponent());

	bAttack = false;

	MaxHealth = 10000000;
	Health = 1234567;

	WaveCnt = 0;
}

void ALich::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	EnemyMovementState = EEnemyMovementState::EMS_Idle;

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &ALich::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &ALich::AgroSphereOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &ALich::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &ALich::CombatSphereOnOverlapEnd);
}

void ALich::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALich::AgroSphereOnOverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
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

void ALich::AgroSphereOnOverlapEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
}

void ALich::CombatSphereOnOverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	Super::CombatSphereOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor)
	{
		AMain *Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			GetCharacterMovement()->MaxWalkSpeed = 0.f;
			bAttack = true;
			SetEnemyMovementState(EEnemyMovementState::EMS_Attack);

			UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && CombatMontage)
			{
				AnimInstance->Montage_Play(CombatMontage, 1.0f);
				AnimInstance->Montage_JumpToSection(FName("Spell"), CombatMontage);
				// if(Laser)
				// {
				// 	FVector StartLocation = GetActorLocation();
				// 	FRotator ActorRotation = GetActorRotation();

				// 	FVector ForwardVector = ActorRotation.Vector().GetSafeNormal();
				// 	ALichLaser* LichLaser = GetWorld()->SpawnActor<ALichLaser>(Laser, StartLocation + ForwardVector*700.f, GetActorRotation());
				// 	if(LichLaser)
				// 	{
				// 		LichLaser->bAttack = true;
				// 	}
				// }
				// if (Skull)
				// {
				// 	for (int i = 0; i < 10; i++)
				// 	{
				// 		FVector MinRange = FVector(-15.0f, -15.0f, 0.0f);
				// 		FVector MaxRange = FVector(15.0f, 15.0f, 0.0f);

				// 		FVector RandomLocation = FMath::RandPointInBox(FBox(MinRange, MaxRange));

				// 		FVector StartLocation = GetActorLocation();
				// 		FRotator ActorRotation = GetActorRotation();

				// 		FVector ForwardVector = ActorRotation.Vector().GetSafeNormal();
				// 		StartLocation.Z += 75.f;
				// 		StartLocation += (RandomLocation*100.f);
				// 		ALichSkull *LichSkull = GetWorld()->SpawnActor<ALichSkull>(Skull, StartLocation + ForwardVector * 100.f, GetActorRotation());
				// 		if (LichSkull)
				// 		{
				// 			LichSkull->bAttack = true;
				// 		}
				// 	}
				// }
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
				if (Wave)
				{
					WaveCnt = 0;
					FourWave();
				}
			}
		}
	}
}

void ALich::CombatSphereOnOverlapEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
}

void ALich::SpellEnd()
{
	SetEnemyMovementState(EEnemyMovementState::EMS_Idle);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
}

void ALich::FourWave()
{
	if (WaveCnt < 5)
	{
		FVector StartLocation = GetActorLocation();
		FRotator ActorRotation = GetActorRotation();
		StartLocation.Z += 50.f;
		
		ActorRotation.Yaw += 90.f;
		StartLocation += (ActorRotation.Vector() * WaveCnt*200.f);
		ALichWave *LichWave = GetWorld()->SpawnActor<ALichWave>(Wave, StartLocation, GetActorRotation());
		if (LichWave)
		{
			LichWave->bAttack = true;
		}
		GetWorldTimerManager().SetTimer(WaveTime, this, &ALich::FourWave, 0.25f);
		WaveCnt++;
	}
}