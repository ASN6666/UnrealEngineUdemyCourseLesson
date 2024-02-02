// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	if(Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if(Pawn)
		{
			MainCharacter = Cast<AMyMainCharacter>(Pawn);
		}
	}

	
	/*  測試
	AActor* OwningActor = GetOwningActor();
	if(OwningActor != nullptr)
	{
		MovementSpeed = OwningActor->GetVelocity().Size();
		UMainAnimInstance* OwningCharacter = Cast<UMainAnimInstance>(OwningActor);
		if(OwningCharacter != nullptr)
		{
			bInAir = Pawn->GetMovementComponent()->IsFalling();
		}
	}
	*/
}
void UMainAnimInstance::UpdateAnimationProperties()
{
		if(Pawn == nullptr)
		{
			Pawn = TryGetPawnOwner();
		}
		if(Pawn) 
		{
			// 取得Pawn 裡的 Velocity （代表著此藍圖會尋找 與角色藍圖鏈接的character的Velocity）
			FVector Speed = Pawn->GetVelocity();
			/*
			 *LateralSpeed 是一个变量或对象，通常是一个三维向量（FVector）
			 *，它代表了一个物体或角色在水平方向上的速度。这意味着它的 Z 轴分量通常为零，因为在这个上下文中，
			 *我们通常不考虑垂直方向上的速度变化。
			 * 
			 */
			FVector LateralSpeed = FVector(Speed.X,Speed.Y,0);
			/*
			 *.Size() 是 FVector 类的一个方法，用来计算向量的长度或大小，
			 *即向量从原点到其终点的距离。在这个情况下，.Size() 将计算 LateralSpeed 向量的长度。
			 * 
			 */
			/*
			 * 第二種寫法	
			FVector LateralSpeed2 = Pawn->GetVelocity();
			LateralSpeed2.Z = 0;
			MovementSpeed = LateralSpeed2.Size();
			
			*/

			MovementSpeed = LateralSpeed.Size(); //size() 來至於GetVelocity
			bInAir = Pawn->GetMovementComponent()->IsFalling();//Pawn = APawn

			//如果 Main Character 為空 將獲取 MyMainCharacter
			
			if(MainCharacter == nullptr)
			{
				MainCharacter = Cast<AMyMainCharacter>(Pawn);
			}
		}
}