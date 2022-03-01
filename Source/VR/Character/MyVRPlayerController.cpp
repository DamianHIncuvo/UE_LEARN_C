// Fill out your copyright notice in the Description page of Project Settings.


#include "MyVRPlayerController.h"
#include "../Enums/AxisSide.h"
#include "../Enums/MenuAnimationStates.h"
#include "../Movement/Rotating/RotateComponent.h"
#include "../Movement/Teleport/TeleportComponent.h"
#include "MyVRPawn.h"
#include "../Grabbing/MyVRHand.h"
#include "../Grabbing/MyGrabbable.h"
#include "../Grabbing/MyGrabber.h"
#include "../GameLogic/MyBaseMenu.h"
#include "MotionControllerComponent.h"

void AMyVRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("MenuToggleLeft", EInputEvent::IE_Released, this, &AMyVRPlayerController::LeftToggleMenu);
	InputComponent->BindAction("MenuToggleRight", EInputEvent::IE_Released, this, &AMyVRPlayerController::RightToggleMenu);
}

void AMyVRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	pawn = Cast<AMyVRPawn>(GetPawn());
}

void AMyVRPlayerController::LeftToggleMenu()
{
	ToggleMenu(false, pawn->leftHand, pawn->rightHand);
}

void AMyVRPlayerController::RightToggleMenu()
{
	ToggleMenu(true, pawn->rightHand, pawn->leftHand);
}

void AMyVRPlayerController::ToggleMenu(bool isRightHand, AMyVRHand* mainHand, AMyVRHand* otherHand)
{
	if (menu == nullptr)
	{
		FTransform projectileTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::OneVector);

		menu = GetWorld()->SpawnActorDeferred<AMyBaseMenu>(menuClass, projectileTransform, nullptr, pawn);
		menu->isActiveMenuHandRight = isRightHand;
		menu->widgetInteractionMain = mainHand->widgetInteraction;
		menu->widgetInteractionOther = otherHand->widgetInteraction;
		menu->motionControllerRef = mainHand->motionController;
		menu->FinishSpawning(projectileTransform);

		pawn->DisableInput(this);
	}
	else
	{
		menu->ChangeState(EMenuAnimationStates::Hiding);
		menu = nullptr;

		pawn->EnableInput(this);
	}
}