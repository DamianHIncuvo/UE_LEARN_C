// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Triggerable.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UTriggerable : public UInterface
{
	GENERATED_BODY()	
};

class ITriggerable
{
	GENERATED_BODY()

public:
	void Trigger();
	bool CanTrigger();
};