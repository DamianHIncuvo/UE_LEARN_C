// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleportComponent.h"
#include "MyVRTeleportVisualizer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MotionControllerComponent.h"
#include "NavigationSystem.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Kismet/GameplayStatics.h"
#include "MyVRTeleportVisualizer.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "../../Character/MyVRPawn.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UTeleportComponent::UTeleportComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UTeleportComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTeleportComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTeleportComponent::StartTeleportTrace()
{
	teleportVisualizer = Cast<AMyVRTeleportVisualizer>(GetWorld()->SpawnActor(teleportVisualizerClass));

	FVector2D playAreaBonds = UHeadMountedDisplayFunctionLibrary::GetPlayAreaBounds();
	teleportVisualizer->Init(FVector(playAreaBonds.X, playAreaBonds.Y, 0.0f));
}

void UTeleportComponent::TeleportTrace()
{
	FVector startPosition = motionController->GetComponentLocation();
	FVector launchVelocity = teleportLaunchSpeed * motionController->GetForwardVector();

	FPredictProjectilePathParams pathParams = FPredictProjectilePathParams(teleportProjectileRadius, startPosition, launchVelocity, 2.0f, UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	FPredictProjectilePathResult pathResult;

	UGameplayStatics::PredictProjectilePath(GetWorld(), pathParams, pathResult);

	FNavLocation outLocation;
	validTeleportLocatioon = pathResult.HitResult.IsValidBlockingHit() && UNavigationSystemV1::GetNavigationSystem(GetWorld())->ProjectPointToNavigation(pathResult.HitResult.ImpactPoint, outLocation);

	if (isVisible != validTeleportLocatioon)
	{
		if (validTeleportLocatioon)
		{
			teleportVisualizer->TurnOnTarget();
		}
		else
		{
			teleportVisualizer->TurnOffTarget();
		}
	}

	if (validTeleportLocatioon)
	{
		FVector relativePlayAreaLocation = pawn->GetActorTransform().InverseTransformPosition(pawn->camera->GetComponentLocation()) * -1.0f;
		relativePlayAreaLocation.Z = 0.0f;

		projectedTeleportLocation = FVector(outLocation.Location.X, outLocation.Location.Y, outLocation.Location.Z - navMeshCellHeight);

		teleportVisualizer->UpdateTarget(
			projectedTeleportLocation,
			relativePlayAreaLocation, 
			pawn->GetActorRotation());
	}

	TArray<FVector> tracePath = TArray<FVector>();
	for (int i = 0; i < pathResult.PathData.Num(); i++)
	{
		tracePath.Add(pathResult.PathData[i].Location);
	}
	tracePath.Insert(startPosition, 0);

	teleportVisualizer->UpdateTrace(tracePath, actorRootSceneComponent->GetComponentLocation());

	isVisible = validTeleportLocatioon;
}

void UTeleportComponent::EndTeleportTrace()
{
	if (isVisible)
	{
		isVisible = false;
	}

	teleportVisualizer->Destroy();
}

bool UTeleportComponent::CanTeleport()
{
	return validTeleportLocatioon;
}

void UTeleportComponent::Teleport()
{
	FVector cameraRelativeLocation = pawn->camera->GetRelativeLocation();
	cameraRelativeLocation.Z = 0.0f;

	float actorYawRotation = pawn->GetActorRotation().Yaw;

	cameraRelativeLocation.RotateAngleAxis(actorYawRotation, FVector::UpVector);

	pawn->TeleportTo(projectedTeleportLocation - cameraRelativeLocation, FRotator(0.0f, actorYawRotation, 0.0f));
}