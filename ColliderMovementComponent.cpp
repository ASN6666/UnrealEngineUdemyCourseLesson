// Fill out your copyright notice in the Description page of Project Settings.


#include "ColliderMovementComponent.h"
void UColliderMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(!PawnOwner || !UpdatedComponent ||ShouldSkipUpdate(DeltaTime) )
	{
		return;
	}
FVector DesiredMobThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f);
	if(!DesiredMobThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMobThisFrame,UpdatedComponent->GetComponentRotation(),true,Hit);
		
		if(Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMobThisFrame,1.f - Hit.Time,Hit.Normal,Hit);
		}
	}
}

