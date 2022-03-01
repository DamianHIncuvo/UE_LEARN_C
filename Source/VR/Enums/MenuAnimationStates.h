// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMenuAnimationStates : uint8
{
    Showing = 0    UMETA(DisplayName = "Showing"),
    Idle = 1    UMETA(DisplayName = "Idle"),
    Hiding = 2    UMETA(DisplayName = "Hiding"),
};