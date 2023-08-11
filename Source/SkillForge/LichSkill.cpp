// Fill out your copyright notice in the Description page of Project Settings.


#include "LichSkill.h"
#include "Lich.h"
#include "Main.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ALichSkill::ALichSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Laser = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Laser"));
	Laser->SetupAttachment(GetRootComponent());

	LaserParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserParticle"));
	LaserParticle->SetupAttachment(Laser);

}

// Called when the game starts or when spawned
void ALichSkill::BeginPlay()
{
	Super::BeginPlay();

	Laser->OnComponentBeginOverlap.AddDynamic(this, &ALichSkill::LaserOverlapBegin);
	
}

// Called every frame
void ALichSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALichSkill::LaserOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
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