// Fill out your copyright notice in the Description page of Project Settings.


#include "MyObject.h"

UMyObject::UMyObject()
{
	Myfloat = 0;
}
void UMyObject::Myfunction()
{
	UE_LOG(LogTemp, Warning, TEXT("This is Warning Text"));
}