// Fill out your copyright notice in the Description page of Project Settings.


#include "MyVRPawn.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "MyVRPlayerController.h"
#include "Components/ChildActorComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "../Enums/AxisSide.h"
#include "../Enums/MenuAnimationStates.h"
#include "../Grabbing/MyVRHand.h"
#include "../Grabbing/MyGrabber.h"
#include "../Grabbing/MyGrabbable.h"
#include "../Character/MyVRPlayerController.h"
#include "../Movement/Teleport/TeleportComponent.h"
#include "../Movement/Rotating/RotateComponent.h"
#include "../InputProcessors/InputRotateProcessor.h"
#include "../InputProcessors/InputTeleportProcessor.h"
#include "../Triggerable.h"
#include "../GameLogic/MyBaseMenu.h"

// Sets default values
AMyVRPawn::AMyVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneRootPlayerRoom = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(sceneRootPlayerRoom);

	camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	camera->SetupAttachment(sceneRootPlayerRoom);

	rotateComponent = CreateDefaultSubobject<URotateComponent>("RotateComponent");
	AddOwnedComponent(rotateComponent);
	rotateComponent->actorRootSceneComponent = sceneRootPlayerRoom;
	rotateComponent->camera = camera;

	teleportComponent = CreateDefaultSubobject<UTeleportComponent>("TeleportComponent");
	AddOwnedComponent(teleportComponent);
	teleportComponent->actorRootSceneComponent = sceneRootPlayerRoom;
	teleportComponent->pawn = this;
}

// Called when the game starts or when spawned
void AMyVRPawn::BeginPlay()
{
	Super::BeginPlay();

	leftHand = Cast<AMyVRHand>(GetWorld()->SpawnActor(AMyVRHand::StaticClass()));
	leftHand->SetOwner(this);
	leftHand->K2_AttachRootComponentToActor(this);

	rightHand = Cast<AMyVRHand>(GetWorld()->SpawnActor(AMyVRHand::StaticClass()));
	rightHand->SetOwner(this);
	rightHand->K2_AttachRootComponentToActor(this);
	rightHand->motionController->MotionSource = FName("Right");

#if WITH_EDITOR
	leftHand->motionController->bDisplayDeviceModel = true;
	rightHand->motionController->bDisplayDeviceModel = true;
#endif

	teleportComponent->motionController = rightHand->motionController;

	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Type::Floor);
	}
}

void AMyVRPawn::LeftInputGrab()
{
	leftHand->grabber->InputGrab();
}

void AMyVRPawn::LeftInputRelease()
{
	leftHand->grabber->InputRelease();
}

void AMyVRPawn::RightInputGrab()
{
	rightHand->grabber->InputGrab();
}

void AMyVRPawn::RightInputRelease()
{
	rightHand->grabber->InputRelease();
}

void AMyVRPawn::LeftTriggerInput()
{
	TriggerInput(leftHand);
}

void AMyVRPawn::RightTriggerInput()
{
	TriggerInput(rightHand);
}

void AMyVRPawn::TriggerInput(AMyVRHand* hand)
{
	if (hand->grabber->IsHoldingGrabbable() == false)
		return;

	if (hand->grabber->heldGrabbable->GetOwner()->Implements<UTriggerable>() == false)
		return;

	ITriggerable* iTriggerable = Cast<ITriggerable>(hand->grabber->heldGrabbable->GetOwner());

	if (iTriggerable == nullptr)
		return;

	if (iTriggerable->CanTrigger() == false)
		return;

	iTriggerable->Trigger();
}

void AMyVRPawn::OnRotateAxis(float inputValue)
{
	EAxisSide axisSide = rotateProcessor->Update(inputValue);

	if (axisSide == EAxisSide::None)
		return;

	rotateComponent->SnapTurn(axisSide);
}

void AMyVRPawn::OnTeleportAxis(float inputValue)
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

// Called every frame
void AMyVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("GrabLeft", EInputEvent::IE_Pressed, this, &AMyVRPawn::LeftInputGrab);
	InputComponent->BindAction("GrabLeft", EInputEvent::IE_Released, this, &AMyVRPawn::LeftInputRelease);
	InputComponent->BindAction("GrabRight", EInputEvent::IE_Pressed, this, &AMyVRPawn::RightInputGrab);
	InputComponent->BindAction("GrabRight", EInputEvent::IE_Released, this, &AMyVRPawn::RightInputRelease);
	InputComponent->BindAction("TriggerLeft", EInputEvent::IE_Pressed, this, &AMyVRPawn::LeftTriggerInput);
	InputComponent->BindAction("TriggerRight", EInputEvent::IE_Pressed, this, &AMyVRPawn::RightTriggerInput);

	rotateProcessor = new InputRotateProcessor(0.75f, 0.5f);
	InputComponent->BindAxis("RotateAxis", this, &AMyVRPawn::OnRotateAxis);

	teleportProcessor = new InputTeleportProcessor(0.75f, 0.5f);
	InputComponent->BindAxis("TeleportAxis", this, &AMyVRPawn::OnTeleportAxis);
}
