// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "item_Explosive.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCPPTEST_API Aitem_Explosive : public AItem
{
	GENERATED_BODY()
public:
	Aitem_Explosive();
	
	

	// 不用UFUNCTION 因為 parent 已經有了
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) override;
	
	virtual  void OnEndOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category ="Damage")
	float Damage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Damage")
	TSubclassOf<UDamageType> DamageTypeClass;
	
};
