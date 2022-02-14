// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGrabbablesProvider.h"
#include "../Grabbing/MyGrabbable.h"
#include "Kismet/GameplayStatics.h"
#include "../Grabbing/MyVRHand.h"
#include "MotionControllerComponent.h"

// Sets default values for this component's properties
UMyGrabbablesProvider::UMyGrabbablesProvider()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyGrabbablesProvider::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	hand = Cast<AMyVRHand>(GetOwner());
}


// Called every frame
void UMyGrabbablesProvider::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UMyGrabbable* UMyGrabbablesProvider::GetBestGrabbableToGrab()
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

	return Cast<UMyGrabbable>(nearestgrabbableComponent);
}

