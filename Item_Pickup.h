// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Item_Pickup.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCPPTEST_API AItem_Pickup : public AItem
{
	
	GENERATED_BODY()
public:
	AItem_Pickup();
	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) override;
	
	virtual  void OnEndOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION(BlueprintImplementableEvent,Category = "Pickup")
	void OnPickupBp(class AMyMainCharacter* Target);
	
	
	
};
