// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Collider.generated.h"

UCLASS()
class UNREALCPPTEST_API ACollider : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

	UPROPERTY(EditAnywhere,Category="Mesh")
	UStaticMeshComponent* MyMeshComponent;
	
	UPROPERTY(EditAnywhere,Category="Mesh")
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere,Category="Mesh")
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere,Category="Mesh")
	USpringArmComponent* SpringArm;
	
	//Getter & Setter Mesh Component
	FORCEINLINE UStaticMeshComponent* GetMeshComponent(){return  MyMeshComponent;}
	FORCEINLINE void SetMeshComponent(UStaticMeshComponent* Mesh){MyMeshComponent = Mesh;}
	
	//Getter & Setter Sphere Component
	FORCEINLINE	USphereComponent* GetSphere(){return  SphereComponent;}
	FORCEINLINE void SetSphereComponent(USphereComponent* Myspehere){SphereComponent = Myspehere;}

	FORCEINLINE void  SetSpringArm(USpringArmComponent* Springarm){SpringArm = Springarm;}
	FORCEINLINE USpringArmComponent*GetSpringArm(){return SpringArm; }
	FORCEINLINE void SetCamera (UCameraComponent* camera){Camera = camera;}
	FORCEINLINE UCameraComponent* GetCamera(){return Camera; }

	
	UPROPERTY(VisibleAnywhere,Category = "MovementComponent")
	class UColliderMovementComponent* OurMovementComponent;
	
	virtual UPawnMovementComponent* GetMovementComponent() const override;
	
	void MoveForward(float value);
	void MoveRight(float value);
	
	void PitchCamera(float CameraY);
	void YawCamera(float CameraX);
	
	FVector2D CameraInput;

	
	
};
