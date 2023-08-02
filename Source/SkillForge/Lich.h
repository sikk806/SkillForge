// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Lich.generated.h"

/**
 * 
 */
UCLASS()
class SKILLFORGE_API ALich : public AEnemy
{
	GENERATED_BODY()

public:
	ALich();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Custom")
	class UPointLightComponent* PointLightComponent;
	
};
