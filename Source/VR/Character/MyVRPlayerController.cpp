// Fill out your copyright notice in the Description page of Project Settings.


#include "MyVRPlayerController.h"
#include "../InputProcessors/InputRotateProcessor.h"
#include "../InputProcessors/InputTeleportProcessor.h"
#include "../Enums/AxisSide.h"
#include "../Movement/Rotating/RotateComponent.h"
#include "../Movement/Teleport/TeleportComponent.h"
#include "MyVRPawn.h"
#include "../Grabbing/MyVRHand.h"
#include "../Grabbing/MyGrabbable.h"
#include "../Grabbing/MyGrabber.h"
#include "MotionControllerComponent.h"

void AMyVRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	rotateProcessor = new InputRotateProcessor(0.75f, 0.5f);
	InputComponent->BindAxis("RotateAxis", this, &AMyVRPlayerController::OnRotateAxis);

	teleportProcessor = new InputTeleportProcessor(0.75f, 0.5f);
	InputComponent->BindAxis("TeleportAxis", this, &AMyVRPlayerController::OnTeleportAxis);
}

void AMyVRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AMyVRPawn* pawn = GetPawn<AMyVRPawn>();

	teleportComponent = pawn->teleportComponent;
	rotateComponent = pawn->rotateComponent;

	InputComponent->BindAction("GrabLeft", EInputEvent::IE_Pressed, pawn, &AMyVRPawn::LeftInputGrab);
	InputComponent->BindAction("GrabLeft", EInputEvent::IE_Released, pawn, &AMyVRPawn::LeftInputRelease);
	InputComponent->BindAction("GrabRight", EInputEvent::IE_Pressed, pawn, &AMyVRPawn::RightInputGrab);
	InputComponent->BindAction("GrabRight", EInputEvent::IE_Released, pawn, &AMyVRPawn::RightInputRelease);
}

void AMyVRPlayerController::OnRotateAxis(float inputValue)
{
	EAxisSide axisSide = rotateProcessor->Update(inputValue);

	if (axisSide == EAxisSide::None)
		return;

	rotateComponent->SnapTurn(axisSide);
}

void AMyVRPlayerController::OnTeleportAxis(float inputValue)
{
	ETeleportState teleportState = teleportProcessor->Update(inputValue);

	if (teleportState == ETeleportState::None)
	{
		return;
	}

	switch (teleportState)
	{
	case ETeleportState::Trace:
		teleportComponent->TeleportTrace();
		break;
	case ETeleportState::Teleport:
		teleportComponent->EndTeleportTrace();
		if (teleportComponent->CanTeleport()) teleportComponent->Teleport();
		break;
	case ETeleportState::Activate:
		teleportComponent->StartTeleportTrace();
		teleportComponent->TeleportTrace();
		break;
	}
}