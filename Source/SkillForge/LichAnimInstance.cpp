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
    
}

