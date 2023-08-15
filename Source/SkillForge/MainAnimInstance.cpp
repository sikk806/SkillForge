// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "Main.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/Vector.h"
#include "Math/UnrealMathUtility.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
    // First, Casting Pawn and Main
    if(Pawn == nullptr)
    {
        Pawn = TryGetPawnOwner();
        if(Pawn)
        {
            Main = Cast<AMain>(Pawn);
        }
    }
}

void UMainAnimInstance::UpdateMainAnimation()
{
    if(Pawn == nullptr)
    {
        Pawn = TryGetPawnOwner();
    }
    if(Pawn)
    {
        // Movement Speed
        Direction = CalculateAngleBetweenVectors(FVector(Main->ForwardValue, 0, 0), FVector(0, Main->RightValue, 0));
        if(Main->MovementStatus == EMovementStatus::EMS_Roll)
        {
            Direction = CalculateAngleBetweenVectors(FVector(Main->FV, 0, 0), FVector(0, Main->RV, 0));
        }
        FVector Speed = Pawn->GetVelocity();
        FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
        MovementSpeed = LateralSpeed.Size();

        FVector LocalSpeed = Pawn->GetActorRotation().UnrotateVector(Speed);

        if(Main == nullptr)
        {
            Main = Cast<AMain>(Pawn);
        }
    }
}

// The direction of the vector when given input (in degrees)
float UMainAnimInstance::CalculateAngleBetweenVectors(const FVector& VectorA, const FVector& VectorB)
{
    FVector NormalizedVectorA = VectorA.GetSafeNormal();
    FVector NormalizedVectorB = VectorB.GetSafeNormal();

    FVector SumVector = NormalizedVectorA + NormalizedVectorB;
    FVector NormalizedSumVector = SumVector.GetSafeNormal();

    float Angle = FMath::Atan2(NormalizedSumVector.Y, NormalizedSumVector.X) * (180.0f / PI);

    return Angle;
}
