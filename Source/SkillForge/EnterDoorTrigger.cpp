// Fill out your copyright notice in the Description page of Project Settings.

#include "EnterDoorTrigger.h"
#include "Main.h"
#include "Lich.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieScene.h"

// Sets default values
AEnterDoorTrigger::AEnterDoorTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	BoxComponent->SetupAttachment(RootComponent);

	Door1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door1"));
	Door1->SetupAttachment(RootComponent);

	Door2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door2"));
	Door2->SetupAttachment(RootComponent);

	CheckTime = 0.f;
	bEnterCheck = false;
}

// Called when the game starts or when spawned
void AEnterDoorTrigger::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnterDoorTrigger::OnOverlapBegin);
}

// Called every frame
void AEnterDoorTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckTime += DeltaTime;

	if (bEnterCheck)
	{
		if (CheckTime > 2.f)
		{

			float YawRotationRate = 30.f; // 회전 속도 (0.5도/초)

			DoorRotation1 = Door1->GetRelativeRotation();
			DoorRotation2 = Door2->GetRelativeRotation();

			if (DoorRotation1.Yaw < -139.f || DoorRotation1.Yaw > 90.f)
			{
				Door1->SetRelativeRotation(FRotator(0.f, DoorRotation1.Yaw - YawRotationRate * DeltaTime, 0.f));
			}

			if (DoorRotation2.Yaw > 139.f || DoorRotation2.Yaw < -90.f)
			{
				Door2->SetRelativeRotation(FRotator(0.f, DoorRotation2.Yaw + YawRotationRate * DeltaTime, 0.f));
			}
		}
	}
}

void AEnterDoorTrigger::OnOverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		AMain *Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			bEnterCheck = true;
			CheckTime = 0.f;
			Main->SceneTime = 0.f;
			// Main->bCutScene = true;
			Main->SetActorLocation(this->GetActorLocation());
			FRotator NewRotation = this->GetActorRotation(); // 원하는 회전값
			Main->Controller->SetControlRotation(NewRotation);

			FMovieSceneSequencePlaybackSettings Settings;
			if (LevelSequence)
			{
				ULevelSequencePlayer *LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, Settings, LevelSequenceActor);
				LevelSequencePlayer->Play();
				FTimerHandle AppearanceTime;
				GetWorldTimerManager().SetTimer(AppearanceTime, this, &AEnterDoorTrigger::LichAppearance, 7.0f);
			}
		}
	}
}

void AEnterDoorTrigger::LichAppearance()
{
	FVector StartLocation = GetActorLocation();
	StartLocation.Z += 100.f;
	FRotator ActorRotation = GetActorRotation();

	FVector ForwardVector = ActorRotation.Vector().GetSafeNormal();
	ALich *LichSpawn = GetWorld()->SpawnActor<ALich>(Lich, StartLocation + ForwardVector * 2110.f, GetActorRotation());
}