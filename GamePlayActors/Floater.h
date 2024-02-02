// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floater.generated.h"

UCLASS()
class UNREALCPPTEST_API AFloater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloater();
	
	UPROPERTY(VisibleAnyWhere,BlueprintReadWrite,Category = "ActorMEshComponent")
	UStaticMeshComponent* StaticMesh;
	

	//BeginPlay Set Location
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite,Category = "FloaterVectors")
	FVector SphereLocation;

	//When Dragged Form Editor SetLocation
	UPROPERTY(VisibleInstanceOnly,Blueprintable,Category = "FloaterVectors")
	FVector PlacedLocation;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "Flaoter Varaible")
	bool bInitializeFloaterLocation;

	UPROPERTY(VisibleAnyWhere, Category = "Bool")
	bool MyBool;

	UPROPERTY(EditAnyWhere,BlueprintReadOnly,Category = "Floater Variable")
	FVector InitialDirection;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Floater Variable")
	bool bShouldFloat;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category = "Floater Variables" )
	FVector Origin;

	UPROPERTY(EditInstanceOnly,BlueprintReadWrite,Category = "Floater Variables")
	FRotator RotatorOffset;

	UPROPERTY(EditInstanceOnly,BlueprintReadWrite,Category = "Floater Variables")
	FVector InitialForce;
	
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite,Category = "Floater Variables")
	FVector InitialTorque;


private:
	float RunningTime;

	float BaseZloaction;
public:
	//Ampitude 摆动幅度
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Floater Variables")
	float A;

	//Period 频率
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Floater Variables")
	float B;
	
	//Phase Shift 频率偏移
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Floater Variables")
	float C;

	//Vertical Shift 上下偏移
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Floater Variables")
	float D;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
