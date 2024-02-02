// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"



AWeapon::AWeapon()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	CombatCollision->SetupAttachment(GetRootComponent());
	
	bWeaponParticle = false;
	Damage = 25.f;
	WeaponState = EWeaponState::EWS_PickUp;
	
}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	CombatCollision->OnComponentBeginOverlap.AddDynamic(this,&AWeapon::CombatCollisionOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this,&AWeapon::CombatCollisionOnEndOverlap);


	//使用 Collision 來 控制 出血效果
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if((WeaponState == EWeaponState::EWS_PickUp) && OtherActor)
	{
		AMyMainCharacter* MainCharacter =Cast<AMyMainCharacter>(OtherActor);
		if(MainCharacter)
		{
			MainCharacter->SetActiveOverlappingItem(this);
		}
	}
}

void AWeapon::OnEndOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	AMyMainCharacter* MainCharacter =Cast<AMyMainCharacter>(OtherActor);
	if(MainCharacter)
	{	
		MainCharacter->SetActiveOverlappingItem(nullptr);
	}
}

void AWeapon::CombatCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if(Enemy)
		{
			if(Enemy->HitParticle)
			{
				const USkeletalMeshSocket* WeaponSocket = SkeletalMesh->GetSocketByName("WeaponSocket");
				if(WeaponSocket)
				{
					FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalMesh);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),Enemy->HitParticle,SocketLocation,FRotator(0.f),false);
				}
				
			}
			if(Enemy->HitSound)
			{
				UGameplayStatics::PlaySound2D(this,Enemy->HitSound);
				UGameplayStatics::PlaySound2D(this,SwingSound);
			}
			if(DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Enemy,Damage,WeaponInstigator,this,DamageTypeClass);
			}
		}
	}
}

void AWeapon::CombatCollisionOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AWeapon::Equip(AMyMainCharacter* MainCharacter)
{
	if(IsValid(MainCharacter))
	{
		SetWeaponInstigator(MainCharacter->GetController()); //控制權限 由 MainCharacter 給予
		
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECR_Ignore);
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECR_Ignore);

		SkeletalMesh->SetSimulatePhysics(false);
		
		const USkeletalMeshSocket* RightHandSocket = MainCharacter->GetMesh()->GetSocketByName("R_WeaponSocket");
		if(RightHandSocket)
		{
			RightHandSocket->AttachActor(this,MainCharacter->GetMesh());
			bRotate = false;

			MainCharacter->SetEquippedWeapon(this);
			MainCharacter->SetActiveOverlappingItem(nullptr);

			WeaponState = EWeaponState::EWS_Equipped;
		
		}
		if(OnEquipSound)
		{
			UGameplayStatics::PlaySound2D(this,OnEquipSound);
		}
		if(!bWeaponParticle)
		{
			IdleParticleComponent->Deactivate();
		}
		
	}
}

void AWeapon::ActiveCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
}

void AWeapon::DeActiveCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
