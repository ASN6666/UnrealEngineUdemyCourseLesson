// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Critter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "SpawnVolume.generated.h"

UCLASS()
class UNREALCPPTEST_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();
	
	
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Spawning")
	UBoxComponent* SpawningBox;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category ="Spawning")
	TSubclassOf<AActor> Actor_1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category ="Spawning")
	TSubclassOf<AActor> Actor_2;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category ="Spawning")
	TSubclassOf<AActor> Actor_3;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category ="Spawning")
	TSubclassOf<AActor> Actor_4;

	TArray<TSubclassOf<AActor>>SpawnActorArray;
	
	ACritter* SpawnActor;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure,Category = "Spawning")
	FVector GetSpawnpont();

	UFUNCTION(BlueprintPure,Category = "Spawning")
	TSubclassOf<AActor>  GetSpawnActor();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category = "Spawning")
	void SpawnOurActor(UClass* SpawnClass,const FVector& location);

	
};
