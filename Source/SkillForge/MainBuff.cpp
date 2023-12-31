// Fill out your copyright notice in the Description page of Project Settings.


#include "MainBuff.h"
#include "Main.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AMainBuff::AMainBuff()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Buff = CreateDefaultSubobject<USphereComponent>(TEXT("Buff"));
	Buff->SetupAttachment(RootComponent);

	BuffParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BuffParticle"));
	BuffParticle->SetupAttachment(Buff);

	ShieldParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShieldParticle"));
	ShieldParticle->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMainBuff::BeginPlay()
{
	Super::BeginPlay();

	ShieldParticle->Deactivate();
	GetWorldTimerManager().SetTimer(BuffTimer, this, &AMainBuff::DestroySelf, 1.5f);

	APlayerController* MainController = GetWorld()->GetFirstPlayerController();
	if (MainController)
	{
    	AMain* Main = Cast<AMain>(MainController->GetPawn());
		if(Main)
		{
			Main->BuffDecrementDamage = 0.2f;
		}
	}
	
}

// Called every frame
void AMainBuff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainBuff::DestroySelf()
{
	APlayerController* MainController = GetWorld()->GetFirstPlayerController();
	if (MainController)
	{
    	AMain* Main = Cast<AMain>(MainController->GetPawn());
		if(Main)
		{
			Main->bAttacking = false;
		}
	}
    Destroy();
}

