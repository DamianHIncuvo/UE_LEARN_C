// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGrabber.h"
#include "MyVRHand.h"
#include "MyGrabbablesProvider.h"
#include "MyGrabbable.h"
#include "MotionControllerComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMyGrabber::UMyGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...

	hand = Cast<AMyVRHand>(GetOwner());
}


// Called every frame
void UMyGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMyGrabber::InputGrab()
{
	UMyGrabbable* grabbable = hand->grabbablesProvider->GetBestGrabbableToGrab();

	if (grabbable == nullptr)
		return;

	if (grabbable->IsHolded())
		grabbable->grabberRef->Release();

	switch (grabbable->GrabType)
	{
	case EMyGrabType::None:
		UE_LOG(LogTemp, Error, TEXT("GrabType is not defined for this grab component."))
			return;
	case EMyGrabType::Free:
		FreeGrab(grabbable);
		break;
	case EMyGrabType::Snap:
		SnapGrab(grabbable);
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Something got fucked up hard."))
			return;
	}

	heldGrabbable = grabbable;

	if (IsHoldingGrabbable() == false)
		return;

	grabbable->OnGrab(this, hand->motionController);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayHapticEffect(OnGrabHapticEffect, GetHandFromMotionSource(hand->motionController), 1.0F, false);
}

void UMyGrabber::InputRelease()
{
	if (heldGrabbable == nullptr)
		return;

	switch (heldGrabbable->GrabType)
	{
	case EMyGrabType::None:
		UE_LOG(LogTemp, Error, TEXT("GrabType is not defined for this grab component."))
			return;
	case EMyGrabType::Free:
	case EMyGrabType::Snap:
		Release();
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Something got fucked up hard."))
			return;
	}
}

bool UMyGrabber::IsHoldingGrabbable()
{
	return heldGrabbable != nullptr;
}

void UMyGrabber::FreeGrab(UMyGrabbable* grabbable)
{
	SetPrimitiveCompPhysics(false, grabbable);

	AttachParentToMotionController(grabbable);
}

void UMyGrabber::SnapGrab(UMyGrabbable* grabbable)
{
	SetPrimitiveCompPhysics(false, grabbable);

	AttachParentToMotionController(grabbable);

	FHitResult* OutSweepHitResult = nullptr;

	grabbable->GetOwner()->GetRootComponent()->SetRelativeRotation(grabbable->grabPoint->GetRelativeRotation().GetInverse(), false, OutSweepHitResult, ETeleportType::TeleportPhysics);
	grabbable->GetOwner()->GetRootComponent()->SetWorldLocation(hand->motionController->GetComponentLocation() + (grabbable->grabPoint->GetComponentLocation() - grabbable->GetOwner()->GetRootComponent()->GetComponentLocation()) * -1.0f, false, OutSweepHitResult, ETeleportType::TeleportPhysics);
}

void UMyGrabber::SetPrimitiveCompPhysics(bool bSimulate, UMyGrabbable* grabbable)
{
	Cast<UPrimitiveComponent>(grabbable->GetOwner()->GetRootComponent())->SetSimulatePhysics(bSimulate);
}

void UMyGrabber::AttachParentToMotionController(UMyGrabbable* grabbable)
{
	grabbable->GetOwner()->GetRootComponent()->AttachToComponent(hand->motionController, FAttachmentTransformRules::KeepWorldTransform, FName(TEXT("None")));
}

EControllerHand UMyGrabber::GetHandFromMotionSource(UMotionControllerComponent* MotionController)
{
	// Uwaga, mam wyjebane na przypadki, gdzie ten string jest ustawiony Ÿle lub wcale, po prostu daj mi praw¹ rêkê. (origynalnie z BP)
	if (MotionController->MotionSource == FName(TEXT("Left")))
	{
		return EControllerHand::Left;
	}
	else
	{
		return EControllerHand::Right;
	}
}

void UMyGrabber::Release()
{
	heldGrabbable->GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	SetPrimitiveCompPhysics(true, heldGrabbable);

	heldGrabbable->OnRelease();

	heldGrabbable = nullptr;
}