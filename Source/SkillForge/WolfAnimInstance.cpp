// Fill out your copyright notice in the Description page of Project Settings.


#include "WolfAnimInstance.h"
#include "Wolf.h"
#include "GameFramework/CharacterMovementComponent.h"

void UWolfAnimInstance::NativeInitializeAnimation()
{
    // First, Casting Pawn and Main
    if(Pawn == nullptr)
    {
        Pawn = TryGetPawnOwner();
        if(Pawn)
        {
            Wolf = Cast<AWolf>(Pawn);
        }
    }
}

void UWolfAnimInstance::UpdateWolfAnimation()
{
    if(Pawn == nullptr)
    {
        Pawn = TryGetPawnOwner();
        if(Pawn)
        {
            Wolf = Cast<AWolf>(Pawn);
        }
    }

    if(Pawn)
    {
        FVector Speed = Pawn->GetVelocity();
        FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
        MovementSpeed = LateralSpeed.Size();
    }
}

