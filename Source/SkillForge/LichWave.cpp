// Fill out your copyright notice in the Description page of Project Settings.


#include "LichWave.h"
#include "Lich.h"
#include "Main.h"
#include "MainPlayerController.h"

#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALichWave::ALichWave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bAttack = false;
	bWaveOn = true;
	bMove = false;

	Wave = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Wave"));
	Wave->SetupAttachment(RootComponent);

	WaveParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WaveParticle"));
	WaveParticle->SetupAttachment(Wave);

	WaveWarning = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaveWarning"));
	WaveWarning->SetupAttachment(Wave);

}

// Called when the game starts or when spawned
void ALichWave::BeginPlay()
{
	Super::BeginPlay();

	Wave->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WaveWarning->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Wave->OnComponentBeginOverlap.AddDynamic(this, &ALichWave::CombatOnOverlapBegin);
	
}

// Called every frame
void ALichWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bAttack)
	{
		if(bWaveOn)
		{
			// FRotator ActorRotation = GetActorRotation();
			// ActorRotation.Yaw += 85.f;
			// WaveParticle->SetWorldRotation(ActorRotation);
			WaveOff(DeltaTime);
		}
		FVector NewScale = WaveWarning->GetComponentScale();
		NewScale.X += 0.2f;
		if(NewScale.X > 10.f) 
		{
			NewScale.X = 10.f;

		}
		WaveWarning->SetWorldScale3D(NewScale);
		if(bMove)
		{
			FVector StartLocation = GetActorLocation();
			FRotator ActorRotation = GetActorRotation();
			FVector ForwardVector = ActorRotation.Vector().GetSafeNormal();

			FVector NewLocation = GetActorLocation() + ForwardVector*10.f;

			SetActorLocation(NewLocation);
		}
	}

}

void ALichWave::DestroyWave()
{
	bAttack = false;
	Destroy();
}

void ALichWave::MoveAttack()
{
	bMove = true;
	WaveWarning->SetActive(false);
	Wave->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetWorldTimerManager().SetTimer(WaveTimer, this, &ALichWave::DestroyWave, 1.f);
}

void ALichWave::WaveOff(float DeltaTime)
{
	bWaveOn = false;
	GetWorldTimerManager().SetTimer(HitWaveTimer, this, &ALichWave::MoveAttack, 1.f);
}

void ALichWave::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if(OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if(Main)
		{			
			UGameplayStatics::ApplyDamage(Main, 10.f, AIController, this, DamageTypeClass);
		}
	}
}