// Fill out your copyright notice in the Description page of Project Settings.


#include "MyVRPawn.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "MyVRPlayerController.h"
#include "../Enums/AxisSide.h"
#include "../Grabbing/MyVRHand.h"
#include "../Grabbing/MyGrabber.h"
#include "../Character/MyVRPlayerController.h"
#include "../Movement/Teleport/TeleportComponent.h"
#include "../Movement/Rotating/RotateComponent.h"
#include "Components/ChildActorComponent.h"

// Sets default values
AMyVRPawn::AMyVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneRootPlayerRoom = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(sceneRootPlayerRoom);

	camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	camera->AttachToComponent(sceneRootPlayerRoom, FAttachmentTransformRules::KeepWorldTransform);

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

// Called every frame
void AMyVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}