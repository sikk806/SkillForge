// Fill out your copyright notice in the Description page of Project Settings.


#include "LichLaser.h"
#include "Lich.h"
#include "Main.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ALichLaser::ALichLaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bAttack = false;

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
	Laser->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LaserWarning->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void ALichLaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bAttack)
	{
		FVector NewScale = FVector(LaserWarning->GetComponentScale().X + 0.2f, LaserWarning->GetComponentScale().Y, LaserWarning->GetComponentScale().Z);
		if(NewScale.X > 16.f) 
		{
			NewScale.X = 16.f;
		}
	}

}

void ALichLaser::LaserOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
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