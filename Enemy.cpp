// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "AIController.h"
#include "MyMainCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
//#include "Kismet/KismetSystemLibrary.h"
#include "MainCharacter.h"
#include "MainPlayerController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(600.f);
	AgroSphere->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Ignore); //world dynamic 改成 ignore

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(75.f);
	

	//把BoxComponent 放在腳上
	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Combat Collision"));
	CombatCollision->SetupAttachment(GetMesh(),FName("EnemySocket"));
	
	Health = 75.f;
	MaxHealth = 100.f;
	Damage = 15.f;

	AttackMinTime = 0.5f;
	AttackMaxTime = 3.5f;
	bOverlappingCombatSphere = false;
	
	EnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;
	DeathDelay = 3.f;

	bHaveValidTarget = false;
}

bool AEnemy::Alive()
{
	return GetEnemyMovementStatus()!= EEnemyMovementStatus::EMS_Death;
}



// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	EnemyController = Cast<AAIController>(GetController());

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this,&AEnemy::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this,&AEnemy::AEnemy::AgroSphereOnEndOverlap);
	
	CombatSphere->OnComponentBeginOverlap.AddDynamic(this,&AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this,&AEnemy::CombatSphereOnEndOverlap);

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this,&AEnemy::CombatCollisionOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this,&AEnemy::CombatCollisionOnEndOverlap);
	
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
/*
 *	Agro
 * 
 */

void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && Alive())
	{
		
		AMyMainCharacter* MainCharacter = Cast<AMyMainCharacter>(OtherActor);
		if(MainCharacter)
		{
			MoveToTarget(MainCharacter);
		}
	}
}
//角色 離開 Enemy範圍 EnemyAIController 設定為不會移動 
void AEnemy::AgroSphereOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor)
	{
		AMyMainCharacter* MainCharacter = Cast<AMyMainCharacter>(OtherActor);
		if(MainCharacter) 
		{
			bHaveValidTarget = false;
			if(MainCharacter->CombatTarget == this)
			{
				MainCharacter->SetCombatTarget(nullptr);
			}
			
			MainCharacter->SetHasCombatTarget(false);
			MainCharacter->UpdateCombatTarget();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			
			if(EnemyController)
			{
				EnemyController->StopMovement();
			}
		} 
	}
}
/*
 *	Combat
 * 
 */

void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && Alive())
	{	
		AMyMainCharacter* MainCharacter = Cast<AMyMainCharacter>(OtherActor);
		if(MainCharacter)
		{
			bHaveValidTarget = true;
			MainCharacter->SetCombatTarget(this);//把MainCharacter 的SetCombatTarget 填充為 此敵人資訊
			MainCharacter->SetHasCombatTarget(true);
			
			MainCharacter->UpdateCombatTarget();
			
			CombatTarget = MainCharacter;
			bOverlappingCombatSphere = true;
			float AttackTime = FMath::FRandRange(AttackMaxTime,AttackMaxTime);
			GetWorldTimerManager().SetTimer(AttackTimer,this,&AEnemy::Attack,AttackTime);
		}
	}
}

void AEnemy::CombatSphereOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && OtherComp)
	{	
		AMyMainCharacter* MainCharacter = Cast<AMyMainCharacter>(OtherActor);
		if(MainCharacter)
		{	
			bOverlappingCombatSphere = false;
			MoveToTarget(MainCharacter);
			CombatTarget = nullptr;

			if(MainCharacter->CombatTarget == this)
			{
				MainCharacter->SetCombatTarget(nullptr);	
				MainCharacter->bHasCombatTarget = false;
				MainCharacter->UpdateCombatTarget();
				
			}
			if(MainCharacter->PlayerController)
			{
				USkeletalMeshComponent* MainSkeletal = Cast<USkeletalMeshComponent>(OtherComp);
				if(MainSkeletal)
				{
					MainCharacter->PlayerController->RemoveEnemyHealthBar();
				}
			}
			
			GetWorldTimerManager().ClearTimer(AttackTimer);
		}
	}
}

//移動到目標位置 CallFunction
void AEnemy::MoveToTarget(AMyMainCharacter* Target)
{
	
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);//模式更改為 移動到目標
	if(EnemyController)
	{
		FAIMoveRequest MoveRequest; 
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(10.0f);
		

		FNavPathSharedPtr NavPath; 
		EnemyController->MoveTo(MoveRequest,&NavPath);

	/*
		auto PathPoints = NavPath ->GetPathPoints();
		for (auto Point : PathPoints)
		{
			FVector Location = Point.Location;
			UKismetSystemLibrary::DrawDebugSphere(this,Location,25.f,8,FLinearColor::Red,10.f,0.5f);
		}
	*/
		
	}
}

void AEnemy::CombatCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		AMyMainCharacter* Main = Cast<AMyMainCharacter>(OtherActor);
		if(Main)
		{
			
			if(Main->HitParticle)
			{
				const USkeletalMeshSocket* TipSocket = GetMesh()->GetSocketByName("TipSocket");
				if(TipSocket)
				{
					FVector SocketLocation = TipSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),Main->HitParticle,SocketLocation,FRotator(0.f),false);
				}
				
			}
			if(DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Main,Damage,GetController(),this,DamageTypeClass);
				
			}
		}
	}
}

void AEnemy::CombatCollisionOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void AEnemy::ActiveCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	if(SwingSound)
	{
		UGameplayStatics::PlaySound2D(this,SwingSound);
	}
}

void AEnemy::DeActiveCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void AEnemy::Attack()
{
	if (Alive() && bHaveValidTarget)
	{ 
		if(EnemyController)
		{
			EnemyController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}
		if(!bAttacking)
		{
			bAttacking = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); //使用 AnimInstance
			if(AnimInstance)
			{
				AnimInstance->Montage_Play(CombatMontage,1.35f);
				AnimInstance->Montage_JumpToSection(FName("Attack"),CombatMontage); // 跳去Montage裡 名為 Attack 的Notify
			}
			
		}
		
	}
}

void AEnemy::Die(AActor* Causer)
{
	
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Death);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); //使用 AnimInstance
	if(AnimInstance)
	{
		AnimInstance->Montage_Play(CombatMontage,1.35f);
		AnimInstance->Montage_JumpToSection(FName("Death"),CombatMontage); // 跳去Montage裡 名為 Attack 的Notify
		
	}
	
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bAttacking = false;

	AMyMainCharacter* MainCharacter = Cast<AMyMainCharacter>(Causer);
	if(MainCharacter)
	{
		MainCharacter->UpdateCombatTarget();
		
	}
}


void AEnemy::AttackEnd()
{
	bAttacking = false;
	if(bOverlappingCombatSphere)
	{
		float AttackTime = FMath::FRandRange(AttackMaxTime,AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer,this,&AEnemy::Attack,AttackTime);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,AActor* DamageCauser)
{
	if(Health - DamageAmount <= 0.f)
	{
		Health -=DamageAmount;
		Die(DamageCauser);
	}
	else
	{
		Health -= DamageAmount;
	}
	return  DamageAmount;
}

void AEnemy::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	GetWorldTimerManager().SetTimer(DeathTimer,this,&AEnemy::Disappear,DeathDelay);
}
void AEnemy::Disappear()
{
	Destroy();
}
