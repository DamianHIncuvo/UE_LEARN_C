// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateComponent.h"
#include "../../Enums/AxisSide.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
URotateComponent::URotateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void URotateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URotateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URotateComponent::SnapTurn(EAxisSide axisSide)
{
	float yawDelta = snapTurnDegrees * (axisSide == EAxisSide::Minus ? -1.0f : 1.0f);
	FHitResult* hitResult = new FHitResult();

	FRotator rotationDelta = FRotator(0.0f, yawDelta, 0.0f);
	GetOwner()->AddActorWorldRotation(rotationDelta, false, hitResult, ETeleportType::None);

	FTransform newTransform = FTransform(actorRootSceneComponent->GetComponentRotation() + FRotator(0.0f, yawDelta, 0.0f), actorRootSceneComponent->GetComponentLocation(), FVector::OneVector);
	FTransform cameraRelativeTransform = camera->GetRelativeTransform();
	FVector cameraLocation = camera->GetComponentLocation();
	GetOwner()->SetActorLocation(actorRootSceneComponent->GetComponentLocation() + (cameraLocation - UKismetMathLibrary::ComposeTransforms(cameraRelativeTransform, newTransform).GetLocation()), false, hitResult, ETeleportType::TeleportPhysics);
}