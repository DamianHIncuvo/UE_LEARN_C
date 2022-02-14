// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGrabbable.h"
#include "Components/SceneComponent.h"
#include <Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "MotionControllerComponent.h"

// Sets default values for this component's properties
UMyGrabbable::UMyGrabbable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UMyGrabbable::BeginPlay()
{
	Super::BeginPlay();

	// Chujowy kod. Nie powinno go w ogóle tu byæ. Takie rzeczy powinno siê ustawiaæ w blueprincie. Jeszcze to chujowe za³o¿enie, ¿e jak nie jestem przyczepiony do jakiegoœ prymitywa to pierdol wszystko - ale niesmak po rzutowaniu pozosta³.
	UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

	if (Primitive)
	{
		if (GetOwner()->GetRootComponent()->IsAnySimulatingPhysics())
		{
			bSimulateOnDrop = true;
		}

		Primitive->SetCollisionProfileName(FName(TEXT("PhysicsActor")), true);
	}
}

bool UMyGrabbable::TryGrab(UMotionControllerComponent* MotionController)
{
	switch (GrabType)
	{
	case EMyGrabType::None:
		UE_LOG(LogTemp, Error, TEXT("GrabType is not defined for this grab component."))
			break;
	case EMyGrabType::Free:
		TryFreeGrab(MotionController);
		break;
	case EMyGrabType::Snap:
		TrySnapGrab(MotionController);
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

	MotionControllerRef = MotionController;

	OnGrabbed.Broadcast();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayHapticEffect(OnGrabHapticEffect, GetHandFromMotionSource(MotionControllerRef), 1.0F, false);

	return true;
}

bool UMyGrabbable::TryRelease()
{
	switch (GrabType)
	{
	case EMyGrabType::None:
		UE_LOG(LogTemp, Error, TEXT("GrabType is not defined for this grab component."))
			break;
	case EMyGrabType::Free:
	case EMyGrabType::Snap:
		Release();
		break;
	case EMyGrabType::Custom:
		bIsHeld = false;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Something got fucked up hard."))
			break;
	}

	if (bIsHeld)
	{
		return false;
	}
	else
	{
		OnDropped.Broadcast();

		return true;
	}
}

void UMyGrabbable::TryFreeGrab(UMotionControllerComponent* MotionController)
{
	SetPrimitiveCompPhysics(false);

	// Jak zwykle mam wyjebane na to, ¿e coœ jest Ÿle ustawione.
	if (AttachParentToMotionController(MotionController))
	{
		bIsHeld = true;
	}
}

void UMyGrabbable::TrySnapGrab(UMotionControllerComponent* MotionController)
{
	SetPrimitiveCompPhysics(false);

	// Jak zwykle mam wyjebane na to, ¿e coœ jest Ÿle ustawione.
	if (AttachParentToMotionController(MotionController))
	{
		bIsHeld = true;

		FHitResult* OutSweepHitResult = nullptr;

		GetOwner()->GetRootComponent()->SetRelativeRotation(GetOwner()->GetRootComponent()->GetRelativeRotation().GetInverse(), false, OutSweepHitResult, ETeleportType::TeleportPhysics);
		GetOwner()->GetRootComponent()->SetWorldLocation(((GetOwner()->GetRootComponent()->GetComponentLocation() - GetOwner()->GetRootComponent()->GetComponentLocation()) * -1.0F) + MotionController->GetComponentLocation(), false, OutSweepHitResult, ETeleportType::TeleportPhysics);
	}
}

void UMyGrabbable::Release()
{
	if (bSimulateOnDrop)
	{
		SetPrimitiveCompPhysics(true);
	}
	else
	{
		GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	bIsHeld = false;
}

EControllerHand UMyGrabbable::GetHandFromMotionSource(UMotionControllerComponent* MotionController)
{
	// Uwaga, mam wyjebane na przypadki, gdzie ten string jest ustawiony Ÿle lub wcale, po prostu daj mi praw¹ rêkê.
	if (MotionController->MotionSource == FName(TEXT("Left")))
	{
		return EControllerHand::Left;
	}
	else
	{
		return EControllerHand::Right;
	}
}

void UMyGrabbable::SetPrimitiveCompPhysics(bool bSimulate)
{
	// Naprawdê kurwa?
	UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

	if (Primitive)
	{
		Primitive->SetSimulatePhysics(bSimulate);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GrabComponent->SetSimulatingParent->Cast To PrimitiveComponent FAILED"))
	}
}

bool UMyGrabbable::AttachParentToMotionController(UMotionControllerComponent* MotionController)
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