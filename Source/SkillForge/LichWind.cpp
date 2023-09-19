// Fill out your copyright notice in the Description page of Project Settings.


#include "LichWind.h"
#include "Lich.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"

// Sets default values
ALichWind::ALichWind()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Wind = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Wind"));
	Wind->SetupAttachment(GetRootComponent());

	WindParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WindParticle"));
	WindParticle->SetupAttachment(Wind);

	CheckWind = false;
	WindCycle = false;

}

// Called when the game starts or when spawned
void ALichWind::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();

	Radius = 0.f;
	
}

// Called every frame
void ALichWind::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!CheckWind)
	{
		CheckWind = true;
		//CurrentAngle = 90.f * WindNo;
		
	}
	if(bAttack)
	{
		// 초당 [RotationSpeed]도 회전.
		float RotationSpeed = 90.f;
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation;
		if(!WindCycle)
		{
			Radius += 1.f;
			if(Radius >= 1000.f)
			{
				WindCycle = true;
			}
		}
		else
		{
			Radius -= 10.f;
			if(Radius <= 0.f)
			{
				bAttack = false;
				Destroy();
			}
		}
		switch(WindNo)
		{
			case 0:
				NewLocation = InitialLocation + FVector(FMath::Cos(FMath::DegreesToRadians(CurrentAngle)), FMath::Sin(FMath::DegreesToRadians(CurrentAngle)), 0.0f) * Radius;
				break;

			case 1:
				NewLocation = InitialLocation + FVector(FMath::Sin(FMath::DegreesToRadians(CurrentAngle)), -FMath::Cos(FMath::DegreesToRadians(CurrentAngle)), 0.0f) * Radius;
				break;

			case 2:
				NewLocation = InitialLocation + FVector(-FMath::Cos(FMath::DegreesToRadians(CurrentAngle)), -FMath::Sin(FMath::DegreesToRadians(CurrentAngle)), 0.0f) * Radius;
				break;

			case 3:
				NewLocation = InitialLocation + FVector(-FMath::Sin(FMath::DegreesToRadians(CurrentAngle)), FMath::Cos(FMath::DegreesToRadians(CurrentAngle)), 0.0f) * Radius;
				break;
		
		
		}


		SetActorLocation(NewLocation);

		CurrentAngle += RotationSpeed * DeltaTime;

		if (CurrentAngle >= 360.0f)
		{
			CurrentAngle = 0.0f;
		}

	}
}

