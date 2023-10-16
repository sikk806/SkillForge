// Fill out your copyright notice in the Description page of Project Settings.


#include "LichSwordPattern.h"
#include "Weapon.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALichSwordPattern::ALichSwordPattern()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BlackSwordHit = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BlackSwordHit"));
	BlackSwordHit->SetupAttachment(RootComponent);

	WhiteSwordHit = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WhiteSwordHit"));
	WhiteSwordHit->SetupAttachment(RootComponent);

	BlackSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlackSword"));
	BlackSword->SetupAttachment(BlackSwordHit);

	WhiteSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WhiteSword"));
	WhiteSword->SetupAttachment(WhiteSwordHit);

	PoisonBombHit = CreateDefaultSubobject<USphereComponent>(TEXT("PoisonBombHit"));
	PoisonBombHit->SetupAttachment(RootComponent);

	PoisonBomb = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PoisonBomb"));
	PoisonBomb->SetupAttachment(PoisonBombHit);

	BlackSafeZone = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BlackSafeZone"));
	BlackSafeZone->SetupAttachment(BlackSwordHit);

	WhiteSafeZone = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WhiteSafeZone"));
	WhiteSafeZone->SetupAttachment(WhiteSwordHit);

	WhiteSwordHitCnt = 5;
	BlackSwordHitCnt = 5;
}

// Called when the game starts or when spawned
void ALichSwordPattern::BeginPlay()
{
	Super::BeginPlay();

	BlackSword->OnComponentBeginOverlap.AddDynamic(this, &ALichSwordPattern::OnBlackSwordOverlapBegin);
	WhiteSword->OnComponentBeginOverlap.AddDynamic(this, &ALichSwordPattern::OnWhiteSwordOverlapBegin);

	BlackSafeZone->Deactivate();
	WhiteSafeZone->Deactivate();
}

// Called every frame
void ALichSwordPattern::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALichSwordPattern::OnBlackSwordOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if(OtherActor)
	{
		AWeapon* Weapon = Cast<AWeapon>(OtherActor);
		if(Weapon)
		{
			if(BlackSwordHitCnt > 0)
			{
				BlackSwordHitCnt--;
			}
			if(BlackSwordHitCnt == 0)
			{
				BlackSword->DestroyComponent();
				BlackSafeZone->Activate();
			}
		}
	}
}

void ALichSwordPattern::OnWhiteSwordOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if(OtherActor)
	{
		AWeapon* Weapon = Cast<AWeapon>(OtherActor);
		if(Weapon)
		{
			if(WhiteSwordHitCnt > 0)
			{
				WhiteSwordHitCnt--;
			}
			if(WhiteSwordHitCnt == 0)
			{
				WhiteSword->DestroyComponent();
				WhiteSafeZone->Activate();
			}
		}
	}
}
