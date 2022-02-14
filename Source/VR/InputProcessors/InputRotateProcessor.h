// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Enums/AxisSide.h"
#include "CoreMinimal.h"

/**
 * 
 */
class VR_API InputRotateProcessor
{
public:
	InputRotateProcessor();
	InputRotateProcessor(float activationDeadzone, float deactivationDeadzone);
	~InputRotateProcessor();

	EAxisSide Update(float newValue);

private:
	float activationDeadzone;
	float deactivationDeadzone;
	bool isActiveLeft;
	bool isActiveRight;
};