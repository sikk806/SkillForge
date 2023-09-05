// Fill out your copyright notice in the Description page of Project Settings.


#include "Wolf.h"
#include "Main.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"

AWolf::AWolf()
{
    WalkMaxSpeed = 200.f;
    RunMaxSpeed = 350.f;

    GetCharacterMovement()->MaxWalkSpeed = 350.f;
}

void AWolf::BeginPlay()
{
    Super::BeginPlay();

    AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AWolf::AgroSphereOnOverlapBegin);

}

void AWolf::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AWolf::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::AgroSphereOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if(OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if(Main)
		{
			
		}
	}

}

void AWolf::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::CombatSphereOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if(OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if(Main)
		{
			
		}
	}
}

void AWolf::SetMovementState(EEnemyMovementState Status)
{
    if(Status == EEnemyMovementState::EMS_MoveToTarget)
    {
        GetCharacterMovement()->MaxWalkSpeed = RunMaxSpeed;
    }
}