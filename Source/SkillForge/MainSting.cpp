// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSting.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"

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

	GetWorldTimerManager().SetTimer(StingTimer, this, &AMainSting::DestroySelf, 0.6f);
	
}

// Called every frame
void AMainSting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainSting::DestroySelf()
{
    Destroy();
}



