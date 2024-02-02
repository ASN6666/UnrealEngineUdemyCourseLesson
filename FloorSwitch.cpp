// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitch.h"

#include "NavigationSystemTypes.h"

// Sets default values
AFloorSwitch::AFloorSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	/*
	 *     TriggerBox 碰撞設定
	 *		SetCollisionEnabled 設定碰撞模式 為 可以重疊和碰撞 但 忽略物理與碰撞計算
	 *		SetCollisionObjectType 設定 OnjectColliison 在世界中是什麼碰撞狀態
	 */
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);//在全部Channel
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);//設定一個Channel  以覆蓋上面的 “ SetCollisionResponseToAllChannels ”
	TriggerBox->SetLineThickness(1.f);
	
	TriggerBox->SetBoxExtent(FVector(62.f,62.f,32.f));
	
	FloorSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorSwitch"));
	FloorSwitch->SetupAttachment(GetRootComponent());

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(GetRootComponent());

	SwitchTime = 2.f;
}

// Called when the game starts or when spawned
void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this,&AFloorSwitch::OnOverlapBox); 
	TriggerBox->OnComponentEndOverlap.AddDynamic(this,&AFloorSwitch::OnOverlapBoxEnd);
	

	InitialDoorLocation = Door->GetComponentLocation();
	InitialSwitchLocation = FloorSwitch->GetComponentLocation();
	
}

// Called every frame
void AFloorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AFloorSwitch::OnOverlapBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{	
	UE_LOG(LogTemp,Warning,TEXT("OverlapBegin"));
	if(!bCharacterOnSwitch)
	{
		bCharacterOnSwitch = true;
	}
	RaiseDoor();
	LowerfloorSwitch();
}
void AFloorSwitch::OnOverlapBoxEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp,Warning,TEXT("OverlapEnd"));
	bCharacterOnSwitch = false;
	GetWorldTimerManager().SetTimer(SwitchHandleTimer,this,&AFloorSwitch::CloseDoor,SwitchTime);
}
void AFloorSwitch::UpdateDoorLocation(float ZLocation)
{
	FVector NEwLocation = InitialDoorLocation;
	NEwLocation.Z += ZLocation;
	Door->SetWorldLocation(NEwLocation);
}

void AFloorSwitch::UpdateFloorSwitchLocation(float ZLocation)
{
	FVector NEwLocation = InitialSwitchLocation;
	NEwLocation.Z += ZLocation;
	FloorSwitch->SetWorldLocation(NEwLocation);
}
void AFloorSwitch::CloseDoor()
{
	if(!bCharacterOnSwitch)
	{
		bCharacterOnSwitch = false;
		LowerDoor();
		RaisefloorSwitch();
	}
}
