// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FirstSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FCharacterState
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere,Category = "SaveGameData")
	float Health;

	UPROPERTY(VisibleAnywhere,Category = "SaveGameData")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere,Category = "SaveGameData")
	float Stamina;

	UPROPERTY(VisibleAnywhere,Category = "SaveGameData")
	float MaxStamina;

	UPROPERTY(VisibleAnywhere,Category = "SaveGameData")
	int32 Coin;

	UPROPERTY(VisibleAnywhere,Category = "SaveGameData")
	FVector PlayerLocation;

	UPROPERTY(VisibleAnywhere,Category = "SaveGameData")
	FRotator Rotation;

	UPROPERTY(VisibleAnywhere,Category = "SaveGameData")
	FString WeaponName;

	UPROPERTY(VisibleAnywhere,Category = "SaveGameData")
	FString LevelName;
	
};

/**
 * 
 */
UCLASS()
class UNREALCPPTEST_API UFirstSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UFirstSaveGame();
	UPROPERTY(VisibleAnywhere,Category = "Basic")
	FString SlotName;
	
	UPROPERTY(VisibleAnywhere,Category = "Basic")
	int32 UserIndex;

	UPROPERTY(VisibleAnywhere,Category = "Basic")
	FCharacterState CharacterState;
};
