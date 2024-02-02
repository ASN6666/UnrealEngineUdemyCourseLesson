// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "MainPlayerController.generated.h"
/**
 * 
 */
UCLASS()
class UNREALCPPTEST_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	//Reference UMG Asset
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Widget")
	TSubclassOf<UUserWidget>HUDOverlayAsset;

	//variable to Hold the widget after creating it
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Widget")
	UUserWidget* HUDoverlay;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Widget")
	TSubclassOf<UUserWidget> WEnemyHealthBar;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Widget")
	UUserWidget* PauseMenu;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Widget")
	TSubclassOf<UUserWidget> WPauseMenu;

	bool bPauseMenuVisible;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category = "HUD")
	void DisplayPauseMenu();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category = "HUD")
	void RemovePauseMenu();
	
	void TogglePauseMenu();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Widget")
	UUserWidget* EnemyHealthBar;

	bool bEnemyHealthBarVisible;
	FVector EnemyLocation;
	
	void DispalyEnemyHealthBar();
	void RemoveEnemyHealthBar();

	void GameModeOnly();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
};
