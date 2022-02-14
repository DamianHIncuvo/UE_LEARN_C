// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EMyGrabType : uint8
{
    None = 0    UMETA(DisplayName = "None"),
    Snap = 1    UMETA(DisplayName = "Snap"),
    Free = 2    UMETA(DisplayName = "Free"),
    Custom = 3  UMETA(DisplayName = "Custom")
};