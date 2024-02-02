// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMainCharacter.h"

#include "Enemy.h"
#include "EnhancedInputComponent.h"
#include "MainPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"
#include "FirstSaveGame.h"
#include "ItemStorage.h"

// Sets default values
AMyMainCharacter::AMyMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//胶囊体设定
	GetCapsuleComponent()->SetCapsuleSize(40.f,90.f);
	
	
	CameraBloom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBloom"));
	CameraBloom->SetupAttachment(GetRootComponent());
	
	CameraBloom->TargetArmLength = 600.f;
	CameraBloom->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(CameraBloom,USpringArmComponent::SocketName);
	//不用角色控制器 進行控制
	CameraComponent->bUsePawnControlRotation = false;

	//不用control 旋转角色
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//旋转幅度
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	GetCharacterMovement()->bOrientRotationToMovement = true;//启用后角色 会自动转向
	GetCharacterMovement()->RotationRate = FRotator(0.0f,540.f,0.0f); //角色旋转速度 默認 540.f
	
	GetCharacterMovement()->JumpZVelocity = 650.f;//跳跃强度
	GetCharacterMovement()->AirControl = 0.2f;//角色在空中操作的强度

	RunningSpeed = 650.f;
	SprintingSpeed = 950.f;

	InterpSpeed = 15.f;
	bInterpToEnemy = false;
	
	Health = 65.f;
	MaxHealth = 100.f;
	Stamina = 120.f;
	MaxStamina = 150.f;
	Coins = 0;
	DelayTimer = 2.f;

	bShiftKeyDown = false;

	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EstaminaStatus::ESS_Normal;

	bMovingright = false;
	bMovingForward = false;
	
	StaminaDrainRate = 25.f;
	MinSprintStamina = 50.f;

	bLmbDown = false;
	bESCDown = false;
	
	//Increase Health Doonce
	bIncreassHealthDonce = true;
	bHasCombatTarget = false;
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}



// Called when the game starts or when spawned
void AMyMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	FString Map = GetWorld()->GetMapName();
	Map.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	PlayerController = Cast<AMainPlayerController>(GetController());
	//New Game
	/*
	if(Map != "ThirdPersonMap")
	{
		//開始時使用 遊戲輸入模式
		if(PlayerController)
		{
			PlayerController->GameModeOnly();
		}
		LoadGameNoSwitch();
	}
	*/

	//Continue
	LoadGameNoSwitch();
	if(PlayerController)
	{
		PlayerController->GameModeOnly();
	}
}

// Called every frame
void AMyMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(MovementStatus == EMovementStatus::EMS_Death)return;
	
	FTimerHandle FHDelay;
	if(Health < MaxHealth)
	{
		if(bIncreassHealthDonce)
		{
			GetWorldTimerManager().SetTimer(FHDelay,this,&AMyMainCharacter::Incresshealth,DelayTimer);
			bIncreassHealthDonce = false;
			
		}
		
	}


	float DeltaStamina = StaminaDrainRate * DeltaTime; //體力消耗 × 時間
	switch (StaminaStatus)
	{
		case EstaminaStatus::ESS_Normal: //體力正常狀態
		if(bShiftKeyDown)
		{
			if(Stamina - DeltaStamina <= MinSprintStamina) //如果體力小於 50 
			{
				SetStaminaStatus(EstaminaStatus::ESS_BleowMinimum); //設定體力狀態為 小於正常
				Stamina -= DeltaStamina;
			}
			else
			{
				Stamina -= DeltaStamina;
				if(bMovingright || bMovingForward)
				{
					SetMovementStatus(EMovementStatus::EMS_Sprinting);
				}
				else
				{
					SetMovementStatus(EMovementStatus::EMS_Normal);
				}
			}
			
		}
		else //key up
		{
			if(Stamina + DeltaStamina >= MaxStamina) //如果 體力大於 等於 最大體力
			{
				Stamina = MaxStamina; //設定體力 為最大
			}
			else
			{
				Stamina += DeltaStamina; //否則 增加體力
			}
			SetMovementStatus(EMovementStatus::EMS_Normal); //體力狀態為正常體力
		}
		
		break;
	case  EstaminaStatus::ESS_BleowMinimum: //體力小於正常 狀態
		if(bShiftKeyDown)
		{
			if(Stamina - DeltaStamina <= 0.f)	//如果 （正在扣減體力中）小於等於 0
			{
				SetStaminaStatus(EstaminaStatus::ESS_Exhausted); //設定 體力狀態為 消耗完畢
				Stamina = 0;
				SetMovementStatus(EMovementStatus::EMS_Normal); //行動模式變為 正常走路狀態
			}
			else
			{
				Stamina -= DeltaStamina;  //否則 繼續減 體力
				SetMovementStatus(EMovementStatus::EMS_Sprinting); //狀態還是 奔跑狀態 
			}
		}
		else //shift key up
		{
			if(Stamina + DeltaStamina >= MinSprintStamina) //如果 體力大於 最小體力值
			{
				SetStaminaStatus(EstaminaStatus::ESS_Normal); // 體力模式 為 正常狀態
				Stamina += DeltaStamina; //體力值照常 增加
			}
			else
			{
				Stamina += DeltaStamina;	//否者 也繼續增加 體力值
			}
			SetMovementStatus(EMovementStatus::EMS_Normal); //鬆開 按鍵 切換成走路模式
		}
		
		break;
	case  EstaminaStatus::ESS_Exhausted: //當體力的狀態值為空
		if(bShiftKeyDown)
		{
			Stamina = 0.f;
		}
		else
		{
			SetStaminaStatus(EstaminaStatus::ESS_ExhaustedRecovering); //否則 體力狀態 狀態改為瀕臨為空 的狀態
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	case EstaminaStatus::ESS_ExhaustedRecovering:
		if(Stamina + DeltaStamina >= MinSprintStamina)	//如果 體力大於 最小體力值
		{
			SetStaminaStatus(EstaminaStatus::ESS_Normal);	//  體力模式 為 正常狀態
			Stamina += DeltaStamina;
		}
		else
		{
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal); // 行動模式 為 正常走路狀態
		break;
	default:
		;
	}
	if(bInterpToEnemy && CombatTarget) //當可以InterpToEnemy時（True） 和 CombatTarget（怪物欄中 已經填充敵人）
	{
		//return （計算FindLookAtRotation函數）
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());// 取得怪物的坐標 然後 在函數中計算並返回  

		// 設定旋轉速度（緩慢旋轉）
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(),LookAtYaw,DeltaTime,InterpSpeed); 
		SetActorRotation(InterpRotation);
	}
	if(CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();
		if(PlayerController)
		{
			PlayerController->EnemyLocation = CombatTargetLocation;
		}
	}
}

// Called to bind functionality to input
void AMyMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&AMyMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this,&AMyMainCharacter::MoveRight);
	
	//mouse Camera control
	PlayerInputComponent->BindAxis(TEXT("Turn"),this,&AMyMainCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"),this,&AMyMainCharacter::LookUp);

	PlayerInputComponent->BindAxis(TEXT("TurnRate"),this,&AMyMainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"),this,&AMyMainCharacter::LookUpRate);

	//action KeyAction

	//Jumping
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AMyMainCharacter::Jump);
	PlayerInputComponent->BindAction("Jump",IE_Released,this,&ACharacter::StopJumping);

	//Running
	PlayerInputComponent->BindAction("Spirnt",IE_Pressed,this,&AMyMainCharacter::ShiftkeyDown);
	PlayerInputComponent->BindAction("Spirnt",IE_Released,this,&AMyMainCharacter::ShiftkeyUP);

	//mouse
	PlayerInputComponent->BindAction("LMB",IE_Pressed,this,&AMyMainCharacter::LmbDown);
	PlayerInputComponent->BindAction("LMB",IE_Released,this,&AMyMainCharacter::LmbUp);

	PlayerInputComponent->BindAction("ESC",IE_Pressed,this,&AMyMainCharacter::ESCDown);
	PlayerInputComponent->BindAction("ESC",IE_Pressed,this,&AMyMainCharacter::ESCUp);


	//增進輸入
	/*
	 *	1.把 playerinput 轉換成 EnhancedInputComponent
	 *	2.用EnhancedInputComponent 代替 PlayerInputComponent
	 *	3.BindAction函數一樣
	 */
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(inputToInteractive,ETriggerEvent::Triggered,this,&AMyMainCharacter::InterActive);
	
}

void AMyMainCharacter::InterActive()
{
	UE_LOG(LogTemp,Warning,TEXT("Interactive!!!"));
}
bool AMyMainCharacter::CanMove(float value)
{	if(PlayerController)
	{
			return  (value != 0.0f) &&
				    (!bAttacking) &&
					(MovementStatus != EMovementStatus::EMS_Death) &&
					!PlayerController->bPauseMenuVisible;
	}
	return false;
}
void AMyMainCharacter::Turn(float value)
{
	if(CanMove(value))
	{
		AddControllerYawInput(value);
	}
}

void AMyMainCharacter::LookUp(float value)
{
	if(CanMove(value))
	{
		AddControllerPitchInput(value);
	}
}

//取得Camera 的向前向量方向，并實時更新Camera所面向的坐標 同時更新角色面向軸向
void AMyMainCharacter::MoveForward(float value)
{
	
	if(CanMove(value))
	{
		bMovingForward = false;
		if(!bAttacking)
		{
			FRotator Myrotator(0,0,0);
			//find out which forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0.0f,Rotation.Yaw,0.f);

			//寻找本地向前方向
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);  
			AddMovementInput(Direction,value);

			bMovingForward = true;
		}
	}
	
}

void AMyMainCharacter::MoveRight(float value)
{
	bMovingright = false;
	if(CanMove(value))
	{
		//使用Camera 前方向量
		//find out which forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f,Rotation.Yaw,0.f);

		//寻找本地坐标 左右 方向
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction,value);
		bMovingright = true;
	}
}
void AMyMainCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
void AMyMainCharacter::LookUpRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}



/*
 *
 *			INVENTORY
 * 
 */
void AMyMainCharacter::DecrementHealth(float DamageAmount)
{
	
	if(Health - DamageAmount <= 0.f)
	{
		Health -= DamageAmount;
		Die();
	}
	else
	{
		Health -= DamageAmount;
	}
	
}

float AMyMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,AActor* DamageCauser)
{
	if(Health - DamageAmount <= 0.f)
	{
		Health -= DamageAmount;
		Die();
		if(DamageCauser)
		{
			AEnemy* Enemy = Cast<AEnemy>(DamageCauser);
			if(Enemy)
			{
				Enemy->bHaveValidTarget = false;
			}
		}
	}
	else
	{
		Health -= DamageAmount;
		
	}
	
	return DamageAmount;
}


void AMyMainCharacter::Incresshealth()
{
	Health += 5.f;
	/*
	DelayTimer = 2.f;
	if(!bIncreassHealthDonce)
	{
		Health += 5.f;
		bIncreassHealthDonce = true;
	}
	
	if(Health >= MaxHealth)
	{
		DelayTimer = 5.f;
		Health = MaxHealth;
		bIncreassHealthDonce = true;
	}
	*/
}

void AMyMainCharacter::incrementCoins(int32 Amount)
{
	Coins += Amount;
}

void AMyMainCharacter::incrementHealth(float Amount)
{
	if(Health + Amount >= MaxHealth)
	{
		Health = MaxHealth;
	}
	else
	{
		Health += Amount;
	}
}

/*
 *
 *			PLAYER STAT
 * 
 */
void AMyMainCharacter::ShiftkeyDown()
{
	bShiftKeyDown = true;
}
void AMyMainCharacter::ShiftkeyUP()
{
	bShiftKeyDown = false;
}

void AMyMainCharacter::LmbDown()
{	
	bLmbDown = true;
	if(MovementStatus == EMovementStatus::EMS_Death)return; // 當偵測到 同等為 true 時後 return 函數（退出此函數）
	if(PlayerController) if(PlayerController->bPauseMenuVisible)return;
	
	if(ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if(Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
	else if (EquipWeapon)
	{
		Attack();
	}	

}

void AMyMainCharacter::Attack()
{
	if(!bAttacking && MovementStatus != EMovementStatus::EMS_Death)
	{
		bAttacking = true;
		SetbInterpToEnemy(true);
		
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if(AnimInstance && CombatMontage)
		{
			int32 Section = FMath::RandRange(0,1);
			switch (Section)
			{
			case 0:
				AnimInstance->Montage_Play(CombatMontage,2.2f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"),CombatMontage);
				break;
			case 1:
				AnimInstance->Montage_Play(CombatMontage,1.83);
				AnimInstance->Montage_JumpToSection(FName("Attack_2"),CombatMontage);
				break;
			}
			
		}

	}
}

void AMyMainCharacter::Jump()
{
	if(PlayerController)if(PlayerController->bPauseMenuVisible)return;
	if(MovementStatus != EMovementStatus::EMS_Death)
	{
		Super::Jump(); //調用 未覆蓋函數版本
	}
}

void AMyMainCharacter::Die()
{
	if(MovementStatus == EMovementStatus::EMS_Death) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage,1.0f);
		AnimInstance->Montage_JumpToSection(FName("Death"));
	}
	SetMovementStatus(EMovementStatus::EMS_Death);
	
	AEnemy* Enemy = Cast<AEnemy>(GetController());
	if(Enemy)
	{
		Enemy->SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
		Enemy->MoveToTarget(nullptr);
	}
}

void AMyMainCharacter::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void AMyMainCharacter::AttackEnd()
{
	bAttacking = false;
	SetbInterpToEnemy(false);
	
	if(bLmbDown)
	{
		Attack();
	}
}


void AMyMainCharacter::PlaySwingSound()
{
	if(EquipWeapon->SwingSound)
	{
		UGameplayStatics::PlaySound2D(this,EquipWeapon->SwingSound);
	}
}

void AMyMainCharacter::LmbUp()
{
	bLmbDown = false;
}

void AMyMainCharacter::ESCDown()
{
	bESCDown = true;
	if(PlayerController)
	{
		PlayerController->TogglePauseMenu();
	}
}

void AMyMainCharacter::ESCUp()
{
	bESCDown = false;
}


void AMyMainCharacter::ShowPicupLocation()
{
	/*
	for (int32 i = 0 ; i < PickUpLocation.Num(); i++)
	{
		
		UKismetSystemLibrary::DrawDebugSphere(this,PickUpLocation[i],25.f,12,FLinearColor::Red,5.f,2.f);
	}
	*/
	for(auto Location : PickUpLocation)
	{
		UKismetSystemLibrary::DrawDebugSphere(this,Location,25.f,12,FLinearColor::Red,5.f,2.f);		
	}
}

void AMyMainCharacter::SetEquippedWeapon(AWeapon* Weapon)
{
	if(EquipWeapon)
	{
		EquipWeapon->Destroy();
	}
	EquipWeapon = Weapon;
}

void AMyMainCharacter::UpdateCombatTarget()
{
	
	TArray<AActor*>Overlappingactor;
	GetOverlappingActors(Overlappingactor,EnemyFilter); //要過濾的Class

	if(Overlappingactor.Num() == 0) //如果 Array內容為空 
	{
		if(PlayerController)
		{
			PlayerController->RemoveEnemyHealthBar();
			
		}
		return; //跳出函數
	}
	
	AEnemy* ClosestEnemy = Cast<AEnemy>(Overlappingactor[0]);
	if(ClosestEnemy)
	{
		FVector Location = GetActorLocation(); //取得角色location
		//取得最近的敵人 （敵人location - 角色自身的location） //size 是把得出的vector 變成 float
		float MinDistance = (ClosestEnemy->GetActorLocation() - Location).Size();
		
		for (auto Actor : Overlappingactor)
		{
			AEnemy* Enemy = Cast<AEnemy>(Actor);
			if(Enemy)
			{
				float DistanceToActor = (Enemy->GetActorLocation()-Location).Size();
				if(DistanceToActor < MinDistance) //兩個值比較
				{
					MinDistance = DistanceToActor;
					ClosestEnemy = Enemy;
				}
			}
		}
		
		SetCombatTarget(ClosestEnemy);
		
		if(PlayerController)
		{
			PlayerController->DispalyEnemyHealthBar();
		}
		bHasCombatTarget = true;
	}
}

void AMyMainCharacter::SetbInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}

FRotator AMyMainCharacter::GetLookAtRotationYaw(FVector Target) //返回 LookARotationYaw 值
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),Target);
	FRotator LookAtRotationYaw(0.f,LookAtRotation.Yaw,0.f);
	return  LookAtRotationYaw;
}

void AMyMainCharacter::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed; //衝刺速度 
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed; //走路速度
	}
}

void AMyMainCharacter::SwtichLevel(FName LevelName)
{
	UWorld* World =GetWorld();
	if(World)
	{
		FString CurrentLevel = World->GetMapName();  
		FName CurrentLevelName(CurrentLevel); //FName 不能轉化為 FString
		if(CurrentLevelName != LevelName)//如果 本關卡地圖 跟 下個關卡名稱不符 
		{
			UGameplayStatics::OpenLevel(World,LevelName);
		}
	}
}
/*
 *	SavingGameStatus
 * 
 */

void AMyMainCharacter::SaveGame()
{
    UFirstSaveGame * SaveGame = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass())); //UFirstSaveGame::StaticClass() 取得UFirstSaveGame裡的全部變量,struct 和 class 數據
	
	if(SaveGame)
	{
		FString MapName = GetWorld()->GetMapName();
		MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); //刪除前綴
		UE_LOG(LogTemp,Warning,TEXT("%s"),*MapName);
		
		//把角色裡的 Status 裝載到 SaveGame Struct
		SaveGame->CharacterState.Health = Health;
		SaveGame->CharacterState.MaxHealth = MaxHealth;
		SaveGame->CharacterState.Stamina = Stamina;
		SaveGame->CharacterState.MaxStamina = MaxStamina;
		SaveGame->CharacterState.Coin = Coins;
		SaveGame->CharacterState.PlayerLocation = GetActorLocation();
		SaveGame->CharacterState.Rotation = GetActorRotation();
		SaveGame->CharacterState.LevelName = MapName;
		if(EquipWeapon)
		{
			SaveGame->CharacterState.WeaponName = EquipWeapon->Name; //保存角色 手中的武器名字
		}

		
		UGameplayStatics::SaveGameToSlot(SaveGame,SaveGame->SlotName,SaveGame->UserIndex);
	}
}

void AMyMainCharacter::LoadGame(bool Setposition)
{
	UFirstSaveGame * LoadGame = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));
	LoadGame = Cast<UFirstSaveGame>( UGameplayStatics::LoadGameFromSlot(LoadGame->SlotName,LoadGame->UserIndex));

	//把 Struct 裡的參數 提取出到 角色身上
	Health = LoadGame->CharacterState.Health;
	MaxHealth = LoadGame->CharacterState.MaxHealth;
	Stamina = LoadGame->CharacterState.Stamina;
	MaxStamina = LoadGame->CharacterState.MaxStamina;
	Coins = LoadGame->CharacterState.Coin;

	SetMovementStatus(EMovementStatus::EMS_Normal);
	GetMesh()->bPauseAnims = false;
	GetMesh()->bNoSkeletonUpdate = false;
	
	if(WeaponStorage) //當 Weapon Class 啟用時 
	{
		AItemStorage* Weapon= GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
		if(WeaponStorage)
		{
			FString WeaponName = LoadGame->CharacterState.WeaponName; //載入 角色手中武器名字
			if(WeaponName != TEXT("")) //如果武器不等於空
			{
				AWeapon* WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapon->WeaponMap[WeaponName]); //生成 Key 相對的 Actor;
				WeaponToEquip->Equip(this); //裝備到角色身上
				
			}
			
		}
	}

	if(Setposition)
	{
		SetActorLocation(LoadGame->CharacterState.PlayerLocation);
		SetActorRotation(LoadGame->CharacterState.Rotation);
	}
	if(LoadGame->CharacterState.LevelName != TEXT(""))
	{
		FName LevelName(*LoadGame->CharacterState.LevelName);
		SwtichLevel(LevelName);
	}
	
}

void AMyMainCharacter::LoadGameNoSwitch()
{
	
	
	UFirstSaveGame * LoadGame = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));
	LoadGame = Cast<UFirstSaveGame>( UGameplayStatics::LoadGameFromSlot(LoadGame->SlotName,LoadGame->UserIndex));

	//把 Struct 裡的參數 提取出到 角色身上
	Health = LoadGame->CharacterState.Health;
	MaxHealth = LoadGame->CharacterState.MaxHealth;
	Stamina = LoadGame->CharacterState.Stamina;
	MaxStamina = LoadGame->CharacterState.MaxStamina;
	Coins = LoadGame->CharacterState.Coin;

	SetMovementStatus(EMovementStatus::EMS_Normal);
	GetMesh()->bPauseAnims = false;
	GetMesh()->bNoSkeletonUpdate = false;
	
	if(WeaponStorage) //當 Weapon Class 啟用時 
	{
		AItemStorage* Weapon= GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
		if(WeaponStorage)
		{
			FString WeaponName = LoadGame->CharacterState.WeaponName; //載入 角色手中武器名字

			if(Weapon->WeaponMap.Contains(WeaponName))//如果 WeaponMap 中有 收納 武器的名字
				{
					AWeapon* WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapon->WeaponMap[WeaponName]); //生成 Key 相對的 Actor

					WeaponToEquip->Equip(this); //裝備到角色身上
				
				}
		}
	}
}


