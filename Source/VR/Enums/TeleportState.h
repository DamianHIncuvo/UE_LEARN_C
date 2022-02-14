// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
enum class ETeleportState : uint8
{
	None = 0			UMETA(DisplayName = "None"),	// Teleport system does nothing
	Teleport = 1		UMETA(DisplayName = "Teleporting"),	// Teleport system hides visuals and tries teleport pawn
	Trace = 2			UMETA(DislpayName = "Trace"),	// Teleport system is only cares about visual representation
	Activate = 3		UMETA(DisplayName = "Activate")	// Teleport system is activating
};