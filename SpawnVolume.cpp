// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"

#include "AIController.h"
#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	RootComponent = SpawningBox;
	
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	if(Actor_1 && Actor_2 && Actor_3 && Actor_4)
	{
		SpawnActorArray.Add(Actor_1);
		SpawnActorArray.Add(Actor_2);
		SpawnActorArray.Add(Actor_3);
		SpawnActorArray.Add(Actor_4); 
	}
	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}
FVector ASpawnVolume::GetSpawnpont()
{
	FVector Extent = SpawningBox->GetScaledBoxExtent();
	FVector Origin = SpawningBox->GetComponentLocation();

	FVector Point =UKismetMathLibrary::RandomPointInBoundingBox(Origin,Extent);
	return  Point;
}

TSubclassOf<AActor> ASpawnVolume::GetSpawnActor()
{
	if(SpawnActorArray.Num() > 0)
	{
		int32 Selection = FMath::RandRange(0,SpawnActorArray.Num() - 1);

		return  SpawnActorArray[Selection];
	}
	else
	{
		return  nullptr;
	}
}

void  ASpawnVolume::SpawnOurActor_Implementation(UClass* SpawnClass,const FVector& location)
{
	if(SpawnClass)
	{
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParameters;
		
		if(World)
		{
			AActor* Actor= World->SpawnActor<AActor>(SpawnClass,location,FRotator(0.f),SpawnParameters);
			
			AEnemy* Enemy = Cast<AEnemy>(Actor);
			if (Enemy)
			{
				 Enemy->SpawnDefaultController(); //當 AI生成時默認是 SpawnDefaultController 不會是 AIController 
				
		 		AAIController* aiController = Cast<AAIController>(Enemy->GetController()); //所以需要把 怪物的自身Controller轉換成 AIController
		 		if(aiController)//控制器 轉換成功
		 		{
		 			Enemy->EnemyController = aiController; //
		 		}
			}
		}
	}
}

