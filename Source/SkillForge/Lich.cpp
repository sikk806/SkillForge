// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Lich.h"
#include "Kismet/GameplayStatics.h"

ALich::ALich()
{
	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
    PointLightComponent->SetupAttachment(GetRootComponent());

	Laser = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Laser"));
	Laser->SetupAttachment(GetRootComponent());

	LaserParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserParticle"));
	LaserParticle->SetupAttachment(Laser);

	LaserWarning = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserWaning"));
	LaserWarning->SetupAttachment(Laser);

	bAttack = false;

}

void ALich::BeginPlay()
{
    Super::BeginPlay();

    GetCharacterMovement()->MaxWalkSpeed = 400.f;

	// Laser SetVisibility For Can' t See
	Laser->SetVisibility(false);
	LaserParticle->SetVisibility(false);
	LaserWarning->SetVisibility(false);

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &ALich::AgroSphereOnOverlapBegin);

	Laser->OnComponentBeginOverlap.AddDynamic(this, &ALich::LaserOverlapBegin);

	Laser->SetGenerateOverlapEvents(false);
	LaserParticle->SetGenerateOverlapEvents(false);\
}

void ALich::Tick(float DeltaTime)
{
	if(bAttack)
	{
		FVector NewScale = FVector(LaserWarning->GetComponentScale().X + 0.2f, LaserWarning->GetComponentScale().Y, LaserWarning->GetComponentScale().Z);
		if(NewScale.X > 16.f) 
		{
			LaserParticle->ActivateSystem(true); // Particle Replay
			LaserParticle->SetVisibility(true);
			NewScale.X = 16.f;
		}
		else
			LaserWarning->SetWorldScale3D(NewScale);
	}
}

void ALich::LaserOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if(OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if(Main)
		{
			UE_LOG(LogTemp, Warning, TEXT("HIT"));
		}
	}
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
				LaserWarning->SetVisibility(true);
				AnimInstance->Montage_Play(CombatMontage, 1.3f);
				AnimInstance->Montage_JumpToSection(FName("Spell"), CombatMontage);
			}
		}
	}
}