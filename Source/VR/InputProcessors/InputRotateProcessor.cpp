// Fill out your copyright notice in the Description page of Project Settings.

#include "InputRotateProcessor.h"
#include "../Enums/AxisSide.h"

InputRotateProcessor::InputRotateProcessor()
{
}

InputRotateProcessor::InputRotateProcessor(float activationDeadzone,float deactivationDeadzone)
{
	this->activationDeadzone = activationDeadzone;
	this->deactivationDeadzone = deactivationDeadzone;
}

InputRotateProcessor::~InputRotateProcessor()
{
}

EAxisSide InputRotateProcessor::Update(float newValue)
{
	if (isActiveLeft)
	{
		if (newValue > -deactivationDeadzone)
		{
			isActiveLeft = false;
		}
	}
	else if(isActiveRight)
	{
		if (newValue < deactivationDeadzone)
		{
			isActiveRight = false;
		}
	}
	else if (newValue > activationDeadzone)
	{
		isActiveRight = true;

		return EAxisSide::Plus;
	}
	else if (newValue < -activationDeadzone)
	{
		isActiveLeft = true;

		return EAxisSide::Minus;
	}

	return EAxisSide::None;
}