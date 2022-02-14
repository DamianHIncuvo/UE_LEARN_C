// Fill out your copyright notice in the Description page of Project Settings.


#include "MyVRPlayerController.h"
#include "../InputProcessors/InputRotateProcessor.h"
#include "../InputProcessors/InputTeleportProcessor.h"
#include "../Enums/AxisSide.h"
#include "../Movement/Rotating/RotateComponent.h"
#include "../Movement/Teleport/TeleportComponent.h"
#include "MyVRPawn.h"
#include "Kismet/GameplayStatics.h"
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
	AMyVRPawn* pawn = GetPawn<AMyVRPawn>();

	teleportComponent = pawn->teleportComponent;
	rotateComponent = pawn->rotateComponent;
	leftHand = pawn->leftHand;
	rightHand = pawn->rightHand;

	InputComponent->BindAction("GrabLeft", EInputEvent::IE_Pressed, leftHand->grabber, &UMyGrabber::InputGrab);
	InputComponent->BindAction("GrabLeft", EInputEvent::IE_Released, this, &AMyVRPlayerController::OnLeftReleaseInput);
	InputComponent->BindAction("GrabRight", EInputEvent::IE_Pressed, rightHand->grabber, &UMyGrabber::InputGrab);
	InputComponent->BindAction("GrabRight", EInputEvent::IE_Released, this, &AMyVRPlayerController::OnRightReleaseInput);
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

void AMyVRPlayerController::OnLeftGrabInput()
{
	OnGrabInput(leftHand);
}

void AMyVRPlayerController::OnLeftReleaseInput()
{
}

void AMyVRPlayerController::OnRightGrabInput()
{
	OnGrabInput(rightHand);
}

void AMyVRPlayerController::OnRightReleaseInput()
{
}

void AMyVRPlayerController::OnGrabInput(AMyVRHand* hand)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes = TArray< TEnumAsByte<EObjectTypeQuery>>();
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	TArray<AActor*> actorsToIgnore = TArray<AActor*>();

	TArray<FHitResult> results = TArray<FHitResult>();

	UActorComponent* nearestgrabbableComponent = nullptr;

	if (UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		hand->motionController->GetComponentLocation(),
		hand->motionController->GetComponentLocation(),
		hand->GrabRadiusFromGripPosition,
		objectTypes,
		false,
		actorsToIgnore,
		EDrawDebugTrace::None,
		results,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		1.0f))
	{
		float nearestComponentDistance = 1000000000.0f;

		for (int i = 0; i < results.Num(); i++)
		{
			UActorComponent* grabbable = results.GetData()->Actor->GetComponentByClass(UMyGrabbable::StaticClass());

			if (grabbable != nullptr)
			{
				float newDistance = FVector::Distance(grabbable->GetOwner()->GetActorLocation(), hand->GetActorLocation());
				if (newDistance < nearestComponentDistance)
				{
					nearestComponentDistance = newDistance;
					nearestgrabbableComponent = grabbable;
				}
			}
		}
	}

	if (nearestgrabbableComponent == nullptr)
		return;

	UMyGrabbable* grabComponent = Cast<UMyGrabbable>(nearestgrabbableComponent);
	if (grabComponent->TryGrab(hand->motionController))
	{
		hand->holdingGrabbable = grabComponent;
	}
}
