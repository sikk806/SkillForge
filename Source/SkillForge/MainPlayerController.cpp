// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if(HUDOverlayAsset)
    {
        HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
    }
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

    // EnemyHealthBar in ViewPort
    if(WEnemyHealthBar)
    {
        EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
        if(EnemyHealthBar)
        {
            EnemyHealthBar->AddToViewport();
            EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
        }
        FVector2D Alignment(0.f, 0.f);
        EnemyHealthBar->SetAlignmentInViewport(Alignment);
    }
}

void AMainPlayerController::DisplayEnemyHealthBar()
{
    if(EnemyHealthBar)
    {
        UE_LOG(LogTemp, Warning, TEXT("DisplayEnemyHealthBar"));
        bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMainPlayerController::RemoveEnemyHealthBar()
{
    if(EnemyHealthBar)
    {
        UE_LOG(LogTemp, Warning, TEXT("RemoveEnemyHealthBar"));
        bEnemyHealthBarVisible = false;
        EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AMainPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(EnemyHealthBar)
    {
        FVector2D PositionInViewport;
        ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);
        PositionInViewport.Y += 85.f;

        FVector2D SizeInViewport(300.f, 25.f);

        EnemyHealthBar->SetPositionInViewport(PositionInViewport);
        EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
    }
}