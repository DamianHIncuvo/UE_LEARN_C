// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAxisSide : uint8
{
	Minus = 0	UMETA(DisplayName = "Minus"),
	Plus = 1	UMETA(DisplayName = "Plus"),
	None = 2	UMETA(DisplayName = "None")
};