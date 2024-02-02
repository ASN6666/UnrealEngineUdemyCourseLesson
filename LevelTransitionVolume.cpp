// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransitionVolume.h"

#include "MyMainCharacter.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ALevelTransitionVolume::ALevelTransitionVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TransitionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransitionVolume"));
	RootComponent = TransitionVolume;

	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillBoard"));
	BillboardComponent->SetupAttachment(GetRootComponent());

	LevelTransitionName = "Level2";
}

// Called when the game starts or when spawned
void ALevelTransitionVolume::BeginPlay()
{
	Super::BeginPlay();

	TransitionVolume->OnComponentBeginOverlap.AddDynamic(this,&ALevelTransitionVolume::OnOverlapBegin);
	
}

// Called every frame
void ALevelTransitionVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelTransitionVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		AMyMainCharacter* MainCharacter = Cast<AMyMainCharacter>(OtherActor);
		if(MainCharacter)
		{
			MainCharacter->SwtichLevel(LevelTransitionName);
		}
	}
}


