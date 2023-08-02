// Fill out your copyright notice in the Description page of Project Settings.


#include "Lich.h"
#include "Components/PointLightComponent.h"

ALich::ALich()
{
	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
    PointLightComponent->SetupAttachment(GetRootComponent());
}