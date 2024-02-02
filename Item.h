// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Item.generated.h"

UCLASS()
class UNREALCPPTEST_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	//Base Shape Collision
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Item | Collision")
	USphereComponent* CollisionVolume;

	//Base Mesh Component
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Item | Collision")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item | Particles ")
	UParticleSystemComponent* IdleParticleComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item | Particles ")
	UParticleSystem* OverlapParticle;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Sound ")
	USoundCue* OverlapSound;

	// Switch of Rotate Actor
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item | Item Property ")
	bool bRotate;

	// itme Rotate Speed
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item | Item Property ")
	float RotationRate;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	virtual  void OnEndOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
