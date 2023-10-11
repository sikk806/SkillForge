// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Main.h"

#include "MainSwordFalling.h"
#include "Engine/World.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainSwordFalling::AMainSwordFalling()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SwordFall = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordFall"));
	SwordFall->SetupAttachment(GetRootComponent());

	SwordFallBeginParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SwordFallBeginParticle"));
	SwordFallBeginParticle->SetupAttachment(SwordFall);

	SwordFallParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SwordFallParticle"));
	SwordFallParticle->SetupAttachment(SwordFall);

	bAttack = false;
	bCombatOverlap = false;
	cntHit = 0;
}

// Called when the game starts or when spawned
void AMainSwordFalling::BeginPlay()
{
	Super::BeginPlay();

	SFTimer = 0.f;
	SwordFall->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SwordFall->OnComponentBeginOverlap.AddDynamic(this, &AMainSwordFalling::CombatOnOverlapBegin);
	SwordFall->OnComponentEndOverlap.AddDynamic(this, &AMainSwordFalling::CombatOnOverlapEnd);

	GetWorldTimerManager().SetTimer(SwordFallTimer, this, &AMainSwordFalling::DestroySelf, 3.0f);

	
	
}

// Called every frame
void AMainSwordFalling::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bAttack)
	{
		SFTimer += DeltaTime;
		if(SFTimer > 0.6f && (cntHit == 0))
		{
			cntHit = 1;
			for (AActor* Target : HitTargetsSet)
			{
				UGameplayStatics::ApplyDamage(Target, 10.f, AIController, this, DamageTypeClass);
			}
		}
		else if(SFTimer > 0.8f && (cntHit == 1 || cntHit == 0))
		{
			cntHit = 2;
			for (AActor* Target : HitTargetsSet)
			{
				UGameplayStatics::ApplyDamage(Target, 10.f, AIController, this, DamageTypeClass);
			}
		}
		else if(SFTimer > 1.f && (cntHit == 2 || cntHit == 0))
		{
			cntHit = 3;
			for (AActor* Target : HitTargetsSet)
			{
				UGameplayStatics::ApplyDamage(Target, 10.f, AIController, this, DamageTypeClass);
			}
		}
		else if(SFTimer > 1.2f && (cntHit == 3 || cntHit == 0))
		{
			cntHit = 4;
			for (AActor* Target : HitTargetsSet)
			{
				UGameplayStatics::ApplyDamage(Target, 10.f, AIController, this, DamageTypeClass);
			}
		}
		else if(SFTimer > 1.4f && (cntHit == 4 || cntHit == 0))
		{
			cntHit = 5;
			for (AActor* Target : HitTargetsSet)
			{
				UGameplayStatics::ApplyDamage(Target, 10.f, AIController, this, DamageTypeClass);
			}
		}
		else if(SFTimer > 2.1f && (cntHit == 5 || cntHit == 0))
		{
			cntHit = 6;
			for (AActor* Target : HitTargetsSet)
			{
				UGameplayStatics::ApplyDamage(Target, 50.f, AIController, this, DamageTypeClass);
			}
		}
	}
}

void AMainSwordFalling::DestroySelf()
{
    Destroy();
}

void AMainSwordFalling::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if(OtherActor)
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if(Enemy)
		{
			HitTargetsSet.Add(Enemy);
		}
	}
}

void AMainSwordFalling::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor)
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if(Enemy)
		{
			if (HitTargetsSet.Contains(Enemy))
			{
				HitTargetsSet.Remove(Enemy);
			}
		}
	}
}


