// Fill out your copyright notice in the Description page of Project Settings.


#include "InputTeleportProcessor.h"
#include "../Enums/TeleportState.h"

InputTeleportProcessor::InputTeleportProcessor()
{
}

InputTeleportProcessor::InputTeleportProcessor(float activationDeadzone, float deactivationDeadzone)
{
    this->activationDeadzone = activationDeadzone;
    this->deactivationDeadzone = deactivationDeadzone;
}

InputTeleportProcessor::~InputTeleportProcessor()
{
}

ETeleportState InputTeleportProcessor::Update(float newValue)
{
    if (isActive)
    {
        if (newValue < deactivationDeadzone)
        {
            isActive = false;
            return ETeleportState::Teleport;
        }
        else
        {
            return ETeleportState::Trace;
        }
    }
    else
    {
        if (newValue > activationDeadzone)
        {
            isActive = true;
            return ETeleportState::Activate;
        }
        else
        {
            return ETeleportState::None;
        }
    }
}
