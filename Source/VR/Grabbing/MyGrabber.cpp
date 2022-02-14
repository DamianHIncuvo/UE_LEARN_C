// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGrabber.h"
#include "MyVRHand.h"
#include "MyGrabbablesProvider.h"
#include "MyGrabbable.h"
#include "MotionControllerComponent.h"
#include <Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

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

	UMyGrabbable* grabComponent = Cast<UMyGrabbable>(grabbable);

	switch (grabComponent->GrabType)
	{
	case EMyGrabType::None:
		UE_LOG(LogTemp, Error, TEXT("GrabType is not defined for this grab component."))
			break;
	case EMyGrabType::Free:
		FreeGrab(MotionController);
		break;
	case EMyGrabType::Snap:
		SnapGrab(MotionController);
		break;
	case EMyGrabType::Custom:
		bIsHeld = true;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Something got fucked up hard."))
			break;
	}

	if (bIsHeld == false)
		return false;


	if (grabComponent->TryGrab(hand->motionController))
	{
		hand->holdingGrabbable = grabComponent;
	}
}

void UMyGrabber::InputRelease()
{
}

void UMyGrabber::FreeGrab(UMotionControllerComponent* MotionController)
{
	SetPrimitiveCompPhysics(false);

	AttachParentToMotionController(MotionController);
}

void UMyGrabber::SnapGrab(UMotionControllerComponent* MotionController)
{
	SetPrimitiveCompPhysics(false);

	AttachParentToMotionController(MotionController);

	FHitResult* OutSweepHitResult = nullptr;

	GetOwner()->GetRootComponent()->SetRelativeRotation(GetOwner()->GetRootComponent()->GetRelativeRotation().GetInverse(), false, OutSweepHitResult, ETeleportType::TeleportPhysics);
	GetOwner()->GetRootComponent()->SetWorldLocation(((GetOwner()->GetRootComponent()->GetComponentLocation() - GetOwner()->GetRootComponent()->GetComponentLocation()) * -1.0F) + MotionController->GetComponentLocation(), false, OutSweepHitResult, ETeleportType::TeleportPhysics);
}

void UMyGrabber::SetPrimitiveCompPhysics(bool bSimulate)
{
	Cast<UPrimitiveComponent>(hand->GetRootComponent())->SetSimulatePhysics(bSimulate);
}

bool UMyGrabber::AttachParentToMotionController(UMotionControllerComponent* MotionController)
{
	// WeŸ mojego parenta, który najwyraŸniej jest moim rootem i przyczem go to kontrolera.
	bool output = GetOwner()->GetRootComponent()->AttachToComponent(MotionController, FAttachmentTransformRules::KeepWorldTransform, FName(TEXT("None")));

	if (output == false)
	{
		FString warningInfo =
			TEXT("Attaching") +
			UKismetSystemLibrary::GetDisplayName(GetOwner()->GetRootComponent()) +
			TEXT("to") +
			UKismetSystemLibrary::GetDisplayName(MotionController) +
			TEXT("FAILED - object not grabbed");

		UE_LOG(LogTemp, Warning, TEXT("%s"), *warningInfo);
	}

	return output;
}

