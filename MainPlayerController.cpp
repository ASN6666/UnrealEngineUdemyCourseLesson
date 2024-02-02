// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"



void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if(HUDOverlayAsset)
	{
		HUDoverlay = CreateWidget<UUserWidget>(this,HUDOverlayAsset);
		
	}
	HUDoverlay->AddToViewport();
	HUDoverlay->SetVisibility(ESlateVisibility::Visible);

	if(WEnemyHealthBar)
	{
		EnemyHealthBar = CreateWidget<UUserWidget>(this,WEnemyHealthBar);
		if(EnemyHealthBar)
		{
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if(WPauseMenu)
	{
		PauseMenu = CreateWidget<UUserWidget>(this,WPauseMenu);
		if(PauseMenu)
		{
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AMainPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(EnemyHealthBar)
	{
		FVector2D PositonInViewPort;
		ProjectWorldLocationToScreen(EnemyLocation,PositonInViewPort);
		PositonInViewPort.Y -= 85.f;
		
		FVector2D SizeInViewport(300.f,25.f);
		EnemyHealthBar->SetPositionInViewport(PositonInViewPort);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
	}
}

void AMainPlayerController::DisplayPauseMenu_Implementation()
{
	if(PauseMenu)
	{
		FInputModeGameAndUI InputModeGameAndUI;
		SetInputMode(InputModeGameAndUI);
		SetShowMouseCursor(true);
		bPauseMenuVisible = true;
		PauseMenu->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemovePauseMenu_Implementation()
{
	if(PauseMenu)
	{
		GameModeOnly();
		SetShowMouseCursor(false);
		bPauseMenuVisible = false;
		//PauseMenu->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::TogglePauseMenu()
{
	if(bPauseMenuVisible)
	{
		RemovePauseMenu();
	}
	else
	{
		DisplayPauseMenu();
	}
}

void AMainPlayerController::DispalyEnemyHealthBar()
{
	if(EnemyHealthBar)
	{
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
	FVector2D Alignment(0.f,0.f);
	EnemyHealthBar->SetAlignmentInViewport(Alignment);
}

void AMainPlayerController::RemoveEnemyHealthBar()
{
	if(EnemyHealthBar)
	{
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::GameModeOnly()
{
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
}
