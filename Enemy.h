// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus:uint8
{
	EMS_Idle UMETA(DisplayName = "Idle"),
	EMS_MoveToTarget UMETA(DisplayName = "MoveToTarget"),
	EMS_Attacking UMETA(DisplayName = "Attacking"),
	EMS_Death UMETA(DisplayName = "Death"),

	EMS_Max UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class UNREALCPPTEST_API AEnemy : public ACharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Movement")
	EEnemyMovementStatus EnemyMovementStatus;

	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus Status){ EnemyMovementStatus = Status;}
	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus(){return EnemyMovementStatus;}

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AI")
	class USphereComponent* AgroSphere;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AI")
	class USphereComponent* CombatSphere;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AI")
	class AAIController* EnemyController;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "AI")
	bool bOverlappingCombatSphere;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI")
	float Health;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI")
	float MaxHealth;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI")
	float Damage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI")
	class UParticleSystem* HitParticle;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI")
	USoundCue* HitSound;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI")
	USoundCue* SwingSound;
/*
 *	戰鬥
 *	戰鬥
 */

public:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Combat")
	class UBoxComponent* CombatCollision;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Combat")
	UAnimMontage* CombatMontage;

	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Combat")
	float AttackMinTime;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Combat")
	float AttackMaxTime;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Combat")
	TSubclassOf<UDamageType>DamageTypeClass;
	
	bool Alive();

	FTimerHandle DeathTimer;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Combat")
	float DeathDelay;

	UFUNCTION()
	void Disappear();

	UPROPERTY(VisibleAnywhere,Category = "Combat")
	bool bHaveValidTarget;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	virtual  void AgroSphereOnEndOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	virtual  void CombatSphereOnEndOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class AMyMainCharacter* MainCharacter);

	UFUNCTION()
	void CombatCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void CombatCollisionOnEndOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ActiveCollision();
	
	UFUNCTION(BlueprintCallable)
	void DeActiveCollision();
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "AI")
	AMyMainCharacter* CombatTarget;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Combat")
	bool bAttacking;
	
	void Attack();
	void Die(AActor* Causer);
		
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	virtual float TakeDamage(float DamageAmount,struct FDamageEvent const & DamageEvent,class AController * EventInstigator,AActor * DamageCauser)override;
	
	UFUNCTION(BlueprintCallable)
	void DeathEnd();
	
};
