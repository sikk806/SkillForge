// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "Lich.h"
#include "LichLaser.h"

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

}

void ALich::BeginPlay()
{
    Super::BeginPlay();

    GetCharacterMovement()->MaxWalkSpeed = 400.f;

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &ALich::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &ALich::AgroSphereOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &ALich::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &ALich::CombatSphereOnOverlapEnd);
}

void ALich::Tick(float DeltaTime)
{
	// if(bAttack)
	// {
	// 	FVector NewScale = FVector(LaserWarning->GetComponentScale().X + 0.2f, LaserWarning->GetComponentScale().Y, LaserWarning->GetComponentScale().Z);
	// 	if(NewScale.X > 16.f) 
	// 	{
	// 		LaserParticle->ActivateSystem(true); // Particle Replay
	// 		LaserParticle->SetVisibility(true);
	// 		NewScale.X = 16.f;
	// 	}
	// 	else
	// 		LaserWarning->SetWorldScale3D(NewScale);
	// }
}

void ALich::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::AgroSphereOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if(OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if(Main)
		{
			
		}
	}
}

void ALich::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ALich::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::CombatSphereOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if(OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if(Main)
		{
			GetCharacterMovement()->MaxWalkSpeed = 0.f;
			bAttack = true;
			SetEnemyMovementState(EEnemyMovementState::EMS_Attack);

			UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && CombatMontage)
			{
				AnimInstance->Montage_Play(CombatMontage, 1.3f);
				AnimInstance->Montage_JumpToSection(FName("Spell"), CombatMontage);
				LichLaser->bAttack = true;
			}
		}
	}
}
	
void ALich::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}