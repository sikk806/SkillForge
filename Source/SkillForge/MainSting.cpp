// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Main.h"

#include "MainSting.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainSting::AMainSting()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sting = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sting"));
	Sting->SetupAttachment(GetRootComponent());

	StingParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("StingParticle"));
	StingParticle->SetupAttachment(Sting);
}

// Called when the game starts or when spawned
void AMainSting::BeginPlay()
{
	Super::BeginPlay();

	StingTime = 0;

	Sting->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sting->OnComponentBeginOverlap.AddDynamic(this, &AMainSting::CombatOnOverlapBegin);

	GetWorldTimerManager().SetTimer(StingTimer, this, &AMainSting::DestroySelf, 0.6f);
	
}

// Called every frame
void AMainSting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StingTime += 1;
	if(StingTime % 10 == 0)
	{
		for(AActor* Target : HitTargetsSet)
		{
			UGameplayStatics::ApplyDamage(Target, 5.f, AIController, this, DamageTypeClass);
		}
	}
}

void AMainSting::DestroySelf()
{
    Destroy();
}

void AMainSting::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
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



