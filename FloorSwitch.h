// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "FloorSwitch.generated.h"

UCLASS()
class UNREALCPPTEST_API AFloorSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorSwitch();

	/* Overlap For the function to be triggered */
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Box Mesh")
	UBoxComponent* TriggerBox; //Box 為 Unity 中的 BoxCollision

	/*Switch for the character to step on */
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Box Mesh")
	UStaticMeshComponent* FloorSwitch;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Box Mesh")
	UStaticMeshComponent* Door;
	
	UPROPERTY(BlueprintReadWrite,Category = "Switch")
	FVector InitialDoorLocation;

	UPROPERTY(BlueprintReadWrite,Category = "Switch")
	FVector InitialSwitchLocation;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Timer")
	float SwitchTime;
	
	FTimerHandle SwitchHandleTimer;

	void CloseDoor();

	bool bCharacterOnSwitch;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void OnOverlapBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnOverlapBoxEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent,Category = "Floor Switch")//BlueprintImplementableEvent 是指不用在C++ 實現功能
	void RaiseDoor();
	
	UFUNCTION(BlueprintImplementableEvent,Category = "Floor Switch")//BlueprintImplementableEvent 是指不用在C++ 實現功能
	void LowerDoor();

	UFUNCTION(BlueprintImplementableEvent,Category = "Floor Switch")//BlueprintImplementableEvent 是指不用在C++ 實現功能
	void RaisefloorSwitch();
	
	UFUNCTION(BlueprintImplementableEvent,Category = "Floor Switch")//BlueprintImplementableEvent 是指不用在C++ 實現功能
	void LowerfloorSwitch();

	UFUNCTION(BlueprintCallable,Category = "Floor Switch")
	void UpdateDoorLocation(float ZLocation);

	UFUNCTION(BlueprintCallable,Category = "Floor Switch")
	void UpdateFloorSwitchLocation(float ZLocation);
	
};