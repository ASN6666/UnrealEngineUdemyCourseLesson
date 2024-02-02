// Fill out your copyright notice in the Description page of Project Settings.


#include "FlaotingPlatform.h"

// Sets default values
AFlaotingPlatform::AFlaotingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	StartPoint = FVector(0.f);
	EndPoint = FVector(0.f);

	bInterping = false;
	
	InterpSpeed = 4.0f;
	InterpTime = 1.f;
}

// Called when the game starts or when spawned
void AFlaotingPlatform::BeginPlay()
{
	Super::BeginPlay();
	StartPoint = GetActorLocation();
	EndPoint += StartPoint;
	bInterping = false;

	GetWorldTimerManager().SetTimer(InterpTimer,this,&AFlaotingPlatform::ToggleInterping,InterpTime);
	Distance = (EndPoint - StartPoint).Size();
}

// Called every frame
void AFlaotingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(bInterping)
	{
		FVector CurrentLocation = GetActorLocation(); 
		FVector Interp = FMath::VInterpTo(CurrentLocation,EndPoint,DeltaTime,InterpSpeed); 
		SetActorLocation(Interp); //設定location使用 Interp 算法
		
		float Distancetraveled = (GetActorLocation() - StartPoint).Size(); // 計算  行駛距離還剩下多少/多近
		
		if(Distance - Distancetraveled <= 0.1f) 
		{
			bInterping = false;
			GetWorldTimerManager().SetTimer(InterpTimer,this,&AFlaotingPlatform::ToggleInterping,InterpTime);
			SwapVectors(StartPoint,EndPoint);
		}
	}

}
void AFlaotingPlatform::ToggleInterping()
{
	bInterping = !bInterping;
}
void AFlaotingPlatform::SwapVectors(FVector &VecOne , FVector &Vectwo)
{
	FVector Temp = VecOne;
	VecOne = Vectwo;
	Vectwo = Temp;
}