// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Pickup.h"
#include "MyMainCharacter.h"
#include "Kismet/GameplayStatics.h"

AItem_Pickup::AItem_Pickup()
{

}
void AItem_Pickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp,Warning,TEXT("Pickup::OnOverlap"));
	
	AMyMainCharacter * MainCharacter = Cast<AMyMainCharacter>(OtherActor);
	
	
	if (IsValid(MainCharacter))
	{
		OnPickupBp(MainCharacter);
		MainCharacter->PickUpLocation.Add(GetActorLocation());

		if(OverlapParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),OverlapParticle,GetActorLocation(),GetActorRotation(),true);
		}
		if(OverlapSound)
		{
			UGameplayStatics::PlaySound2D(this,OverlapSound);
		}
		Destroy();
	}
}
void AItem_Pickup::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp,Warning,TEXT("Pickup::OnEndOverlap"));
}
