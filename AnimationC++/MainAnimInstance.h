// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UnrealCPPtest/MyMainCharacter.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCPPTEST_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual  void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable,Category = "FunctionProperties")
	void UpdateAnimationProperties();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Movement")
	float MovementSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Movement")
	bool bInAir;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Movement")
	APawn* Pawn;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Movement")
	AMyMainCharacter* MainCharacter;
};
