// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "Critter.generated.h"

UCLASS()
class UNREALCPPTEST_API ACritter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACritter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Mesh")
	UStaticMeshComponent* MyStaticMesh;
	/*
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;
	*/

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="MovementSpeed")
	float MaxSpeed;
private:
	void Moveforward(float Value);
	void MoveRight(float Value);

	FVector CurrentVelocity;
};
