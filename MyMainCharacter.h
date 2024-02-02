// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyMainCharacter.generated.h"

//行動狀態 枚舉
UENUM(BlueprintType)
enum class EMovementStatus : uint8 //unit8 代表著能在 blueprint裡發現他
{
	EMS_Normal UMETA(DisplayName= "Normal"), //UMETA(DisplayName = "") 可以在藍圖裡呼叫列表時所呈現的內容名字
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Death UMETA(DisplayName = "Death"),
	EMS_Max UMETA(DisplayName = "DefaultMax")
	
};

UENUM(BlueprintType)
enum class EstaminaStatus:uint8
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_BleowMinimum UMETA(DisplayName = "BelowMinimum"),
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering"),

	ESS_Max UMETA(Dispaly = "Default_Max")
	
};

UCLASS()
class UNREALCPPTEST_API AMyMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyMainCharacter();
	
	//公開 行動狀態枚舉 變數
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Enums")
	EMovementStatus MovementStatus;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Enums")
	EstaminaStatus StaminaStatus;

	FORCEINLINE void SetStaminaStatus(EstaminaStatus Status){StaminaStatus = Status;}

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Movement")
	float StaminaDrainRate;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Movement")
	float MinSprintStamina;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Camera",meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Camera",meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBloom;
	
	float InterpSpeed;
	bool bInterpToEnemy;
	void SetbInterpToEnemy(bool Interp);

	FRotator GetLookAtRotationYaw(FVector Target);

	//相机旋转
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Camera")
	float BaseTurnRate;

	//相机上下旋转
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Camera")
	float BaseLookUpRate;

	TArray<float>Myfloat;

	// 設定移動狀態 且輸入源來至 行動狀態枚舉
	void SetMovementStatus(EMovementStatus Status);
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Running")
	float RunningSpeed;
	
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Running")
	float SprintingSpeed;
	bool bShiftKeyDown;

	virtual void Jump() override;
	
	void ShiftkeyDown();
	void ShiftkeyUP();
	
	/*
	 *
	 *   戰鬥目標
	 * 
	 */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Combat")
	bool bHasCombatTarget;

	FORCEINLINE void SetHasCombatTarget(bool HasTarget){bHasCombatTarget = HasTarget;}
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Combat")
	class AEnemy* CombatTarget; //取得 敵人資訊
	
	FORCEINLINE void SetCombatTarget(AEnemy* Target){ CombatTarget= Target;}

	UFUNCTION()
	void UpdateCombatTarget();

	/*
	 *
	 *	MouseButton
	 * 
	 */

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Items")
	bool bLmbDown;
	void LmbDown();
	void LmbUp();

	bool bESCDown;
	void ESCDown();
	void ESCUp();
	
	/*
	 *
	 * 生命值
	 * 
	 */
	UFUNCTION(BlueprintCallable,Category = "Health Increass")
	void Incresshealth();
	bool bIncreassHealthDonce;
	float DelayTimer;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Controller")
	class AMainPlayerController* PlayerController;
	
	TArray<FVector> PickUpLocation;
	UFUNCTION(BlueprintCallable)
	void ShowPicupLocation();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Item")
	class AWeapon* EquipWeapon;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Item")
	class AItem* ActiveOverlappingItem;

	void SetEquippedWeapon(AWeapon* Weapon);
	FORCEINLINE AWeapon* GetEquippedWeapon(){return EquipWeapon;}
	FORCEINLINE void SetActiveOverlappingItem(AItem* Item){ActiveOverlappingItem = Item;}

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere,Category = "Combat")
	FVector CombatTargetLocation;


	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Combat")
	TSubclassOf<class AEnemy> EnemyFilter;
	
	/*
	 *
	 * Player Status
	 * 
	 */

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "PlayerStat")
	float Health;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "PlayerStat")
	float MaxHealth;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "PlayerStat")
	float Stamina;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "PlayerStat")
	float MaxStamina;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "PlayerStat")
	int32 Coins;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Combat")
	class UParticleSystem* HitParticle;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Combat")
	class USoundCue* HitSound;

	UFUNCTION(BlueprintCallable,Category = "Combat")
	void DecrementHealth(float DamageAmount);
	
	void Die();

	/*
	 *
	 * 角色數值
	 * 
	 */
	UFUNCTION(BlueprintCallable)
	void incrementCoins(int32 Amount);

	UFUNCTION(BlueprintCallable)
	void incrementHealth(float Amount);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//在藍圖公開變量
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "Enhanced Input")
	class UInputMappingContext* InputMappingContext;

	//在藍圖公開變量
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Enhanced Input")
	UInputAction* inputToInteractive;

	//測試function
	UFUNCTION(BlueprintCallable)
	void InterActive();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*
	 *
	 *	Movement 
	 *	
	 */

	bool CanMove(float value);
	
	void MoveForward(float value);
	void MoveRight(float value);

	bool bMovingForward;
	bool bMovingright;

	void Turn(float value);
	void LookUp(float value);
	
	void TurnAtRate(float Rate);
	void LookUpRate(float Rate);

	FORCEINLINE USpringArmComponent *GetSpringArmComponent()const {return CameraBloom;}
	FORCEINLINE UCameraComponent* GetFollowCamera()const{return  CameraComponent;}

	/*
	 *	Combat
	 * 
	 */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Anims")
	bool bAttacking;
	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Anims")
	class UAnimMontage* CombatMontage;

	UFUNCTION(BlueprintCallable)
	void PlaySwingSound();

	virtual float TakeDamage(float DamageAmount,struct FDamageEvent const & DamageEvent,class AController * EventInstigator,AActor * DamageCauser)override;
	
	void SwtichLevel(FName LevelName);


	/*
	 *
	 *	SavingGameStatus
	 * 
	 */
	
	
	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame(bool Setposition);

	void LoadGameNoSwitch();

	UPROPERTY(EditDefaultsOnly,Category = "SaveData")
	TSubclassOf<class AItemStorage>WeaponStorage;
};
