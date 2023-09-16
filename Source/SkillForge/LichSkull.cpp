// Fill out your copyright notice in the Description page of Project Settings.

#include "LichSkull.h"

#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "TimerManager.h"


// Sets default values
ALichSkull::ALichSkull()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Skull = CreateDefaultSubobject<USphereComponent>(TEXT("Skull"));
	Skull->SetupAttachment(GetRootComponent());

	SkullMuzzle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SkullMuzzle"));
	SkullMuzzle->SetupAttachment(Skull);

	SkullHit = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SkullHit"));
	SkullHit->SetupAttachment(Skull);

	SkullWarning = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkullWarning"));
	SkullWarning->SetupAttachment(Skull);

	bAttack = true;
	bSkullOn = true;
	bSkullBoom = false;

}

// Called when the game starts or when spawned
void ALichSkull::BeginPlay()
{
	Super::BeginPlay();

	SkullMuzzle->Deactivate();
	SkullHit->Deactivate();
	
}

// Called every frame
void ALichSkull::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(bAttack)
	{
		if(bSkullOn)
		{
			SkullMuzzle->Activate();
			SkullOff();
		}
		FVector NewScale = SkullWarning->GetComponentScale();
		NewScale.X += 0.05f;
		NewScale.Z += 0.05f;
		if(NewScale.X > 6.f) 
		{
			NewScale.X = 6.f;
			NewScale.Z = 6.f;
			if(!bSkullBoom)
			{
				bSkullBoom = true;
				SkullHit->Activate();
			}
		}
		SkullWarning->SetWorldScale3D(NewScale);
	}

}

void ALichSkull::SkullOff()
{
	bSkullOn = false;
	GetWorldTimerManager().SetTimer(SkullTimer, this, &ALichSkull::DestroySkull, 2.f);
}

void ALichSkull::DestroySkull()
{
	bAttack = false;
	Destroy();
}

