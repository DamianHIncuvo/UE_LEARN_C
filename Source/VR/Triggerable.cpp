// Fill out your copyright notice in the Description page of Project Settings.


#include "Triggerable.h"

bool ITriggerable::CanTrigger()
{
	UE_LOG(LogTemp, Error, TEXT("ITriggerable::CanTrigger"));
	return false;
}

void ITriggerable::Trigger()
{
	UE_LOG(LogTemp, Error, TEXT("ITriggerable::Trigger"));
}