// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Enums/TeleportState.h"

/**
 * 
 */
class VR_API InputTeleportProcessor
{
public:
	InputTeleportProcessor();
	InputTeleportProcessor(float activationDeadzone, float deactivationDeadzone);
	~InputTeleportProcessor();

	ETeleportState Update(float newValue);

private:
	bool isActive = false;
	float activationDeadzone;
	float deactivationDeadzone;
};
