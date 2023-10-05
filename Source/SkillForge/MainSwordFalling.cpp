// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSwordFalling.h"
#include "Engine/World.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"

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

}

// Called when the game starts or when spawned
void AMainSwordFalling::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainSwordFalling::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainSwordFalling::DestroySelf()
{
    Destroy();
}


