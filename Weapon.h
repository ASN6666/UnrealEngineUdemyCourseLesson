// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "MyMainCharacter.h"
#include "Weapon.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum  class EWeaponState:uint8
{
	EWS_PickUp UMETA(Displayname = "PickUp"),
	EWS_Equipped UMETA(Displayname = "Equipped"),
	EWS_DefaultMax UMETA(Displayname = "DefaultMax"),
};

UCLASS()
class UNREALCPPTEST_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();

	UPROPERTY(EditDefaultsOnly,Category = "SavedData")
	FString Name;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Item | Weapon")
	EWeaponState WeaponState;

	FORCEINLINE void SetWeaponState(EWeaponState State ){WeaponState = State;}
	FORCEINLINE EWeaponState GetWeaponState(){return  WeaponState;}
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item|Sound")
	USoundCue* OnEquipSound;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item|Sound")
	USoundCue* SwingSound;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "SkeletalMesh")
	USkeletalMeshComponent *SkeletalMesh; //劍的模型骨架
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Item | Combat")
	class UBoxComponent* CombatCollision;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Item | Combat")
	float Damage;

	
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) override;
	virtual  void OnEndOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	UFUNCTION()
	void CombatCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
	UFUNCTION()
	void CombatCollisionOnEndOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	void Equip(AMyMainCharacter* MainCharacter);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "ItemParticle")
	bool bWeaponParticle;

	
	UFUNCTION(BlueprintCallable)
	void ActiveCollision();

	UFUNCTION(BlueprintCallable)
	void DeActiveCollision();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Combat")
	AController* WeaponInstigator;

	FORCEINLINE void SetWeaponInstigator(AController* Inst){ WeaponInstigator = Inst;}

	
};
