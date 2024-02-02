// Fill out your copyright notice in the Description page of Project Settings.


#include "item_Explosive.h"

#include "Enemy.h"
#include "MyMainCharacter.h"
#include "ShaderPrintParameters.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"

Aitem_Explosive::Aitem_Explosive()
{
	Damage = 15.f;
}
 void Aitem_Explosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	Super::OnOverlapBegin( OverlappedComponent,  OtherActor, OtherComp, OtherBodyIndex,  bFromSweep, SweepResult);
	if(OtherActor)
	{
		AMyMainCharacter * MainCharacter = Cast<AMyMainCharacter>(OtherActor);
		AEnemy* Enemy  = Cast<AEnemy>(OtherActor);
		
		if (MainCharacter|| Enemy)
		{
			
			if(OverlapParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),OverlapParticle,GetActorLocation(),GetActorRotation(),true);
			}
			if(OverlapSound)
			{
				UGameplayStatics::PlaySound2D(this,OverlapSound);
			}
			UGameplayStatics::ApplyDamage(OtherActor,Damage,nullptr,this,DamageTypeClass);
			Destroy();
		}
	}
	
}
 void Aitem_Explosive::OnEndOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent,OtherActor,OtherComp,OtherBodyIndex);
	UE_LOG(LogTemp,Warning,TEXT("Explo::On Overlap End"));
}
