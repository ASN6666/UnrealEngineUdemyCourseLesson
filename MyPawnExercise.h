// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ColliderMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyPawnExercise.generated.h"

UCLASS()
class UNREALCPPTEST_API AMyPawnExercise : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawnExercise();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,Category = "Mesh")
	UStaticMeshComponent* MyStaticMeshComponent;

	UPROPERTY(EditAnywhere,Category = "SpringArm")
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(EditAnywhere,Category ="CameraComponent")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere,Category = "MovementComponent")
	UColliderMovementComponent* MovementComponent;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void MoveForward(float InputF);
	void MoveRight(float InputR);

	FVector2d MouseInput;
	void MouseX(float AxisX);
	void MouseY(float AxisY);

	UPROPERTY(EditInstanceOnly,Category ="CharacterSpeed")
	float CharacterSpeed;
};
