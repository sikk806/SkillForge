// Fill out your copyright notice in the Description page of Project Settings.


#include "LichLaser.h"
#include "Lich.h"
#include "Main.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALichLaser::ALichLaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bAttack = false;
	bLaserOn = false;
	bLaserHit = false;

	Laser = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Laser"));
	Laser->SetupAttachment(GetRootComponent());

	LaserParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserParticle"));
	LaserParticle->SetupAttachment(Laser);

	LaserWarning = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserWaning"));
	LaserWarning->SetupAttachment(Laser);

}

// Called when the game starts or when spawned
void ALichLaser::BeginPlay()
{
	Super::BeginPlay();
	
	Laser->OnComponentBeginOverlap.AddDynamic(this, &ALichLaser::LaserOverlapBegin);
	Laser->OnComponentEndOverlap.AddDynamic(this, &ALichLaser::LaserOverlapEnd);

	Laser->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LaserWarning->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Target = nullptr;
}

// Called every frame
void ALichLaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bAttack)
	{
		if(bLaserOn)
		{
			LaserOff();
		}
		FVector NewScale = LaserWarning->GetComponentScale();
		NewScale.X += 0.2f;
		if(NewScale.X > 16.f) 
		{
			NewScale.X = 16.f;
			bLaserOn = true;
		}
		LaserWarning->SetWorldScale3D(NewScale);
	}

	if(bLaserHit)
	{
		if(Target != nullptr)	UGameplayStatics::ApplyDamage(Target, 0.2f, AIController, this, DamageTypeClass);
	}

}

void ALichLaser::LaserOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if(OtherActor && !bLaserHit)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if(Main)
		{
			bLaserHit = true;
			Target = Main;
		}
	}
}

void ALichLaser::LaserOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if(Main)
		{
			bLaserHit = false;
			Target = nullptr;
		}
	}
}

void ALichLaser::LaserOff()
{
	bLaserOn = false;
	Laser->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	bAttack = false;
	GetWorldTimerManager().SetTimer(LaserTimer, this, &ALichLaser::DestroyLaser, 1.25f);
}

void ALichLaser::DestroyLaser()
{
	Destroy();
}