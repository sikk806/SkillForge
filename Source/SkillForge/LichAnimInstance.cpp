// Fill out your copyright notice in the Description page of Project Settings.


#include "LichAnimInstance.h"
#include "Lich.h"
#include "GameFramework/CharacterMovementComponent.h"

void ULichAnimInstance::NativeInitializeAnimation()
{
    // First, Casting Pawn and Main
    if(Pawn == nullptr)
    {
        Pawn = TryGetPawnOwner();
        if(Pawn)
        {
            Lich = Cast<ALich>(Pawn);
        }
    }
}

void ULichAnimInstance::UpdateLichAnimation()
{
    if(Pawn == nullptr)
    {
        Pawn = TryGetPawnOwner();
        if(Pawn)
        {
            Lich = Cast<ALich>(Pawn);
        }
    }

    if(Pawn)
    {
        FVector Speed = Pawn->GetVelocity();
        FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
        MovementSpeed = LateralSpeed.Size();
    }
}

