// Fill out your copyright notice in the Description page of Project Settings.


#include "Lich.h"

// Sets default values
ALich::ALich()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALich::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALich::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALich::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

