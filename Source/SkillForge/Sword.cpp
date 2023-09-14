// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Main.h"
#include "Enemy.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASword::ASword()
{
    SwordState = EWeaponState::EWS_Pickup;

    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    SkeletalMesh->SetupAttachment(GetRootComponent());

    CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
    CombatCollision->SetupAttachment(GetRootComponent());

    CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

    Damage = 25.f;

}

void ASword::BeginPlay()
{
    Super::BeginPlay();

    CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &ASword::CombatOnOverlapBegin);
    CombatCollision->OnComponentEndOverlap.AddDynamic(this, &ASword::CombatOnOverlapEnd);
}

void ASword::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    if(OtherActor)
    {
        AMain* Main = Cast<AMain>(OtherActor);
        if(Main)
        {
            Equip(Main);
        }
    }
}

void ASword::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}

void ASword::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if(OtherActor)
    {
        AEnemy* Enemy = Cast<AEnemy>(OtherActor);
        if(Enemy)
        {
            if(Enemy->HitParticles)
            {
                const USkeletalMeshSocket* WeaponSocket = SkeletalMesh->GetSocketByName("WeaponSocket");
                if(WeaponSocket)
                {
                    FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalMesh);
                    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticles, GetActorLocation(), FRotator(0.f), true);
                }
            }
            if(DamageTypeClass)
            {
                UGameplayStatics::ApplyDamage(Enemy, Damage, WeaponInstigator, this, DamageTypeClass);
            }
        }
    }
}

void ASword::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ASword::ActivateCollision()
{

}

void ASword::DeactivateCollision()
{
    
}

// For attach Character Socket	
void ASword::Equip(class AMain* Char)
{
    if(Char)
    {
        SetInstigator(Char->GetController());
        // Not to Collision With Camera And Sword.
        SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
        SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

        SkeletalMesh->SetSimulatePhysics(false);

        const USkeletalMeshSocket* RightHandSocket = Char->GetMesh()->GetSocketByName("RightHandSocket");
        if(RightHandSocket)
        {
            RightHandSocket->AttachActor(this, Char->GetMesh());
            Char->SetEquippedWeapon(this);
        }
    }
}