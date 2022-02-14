// Fill out your copyright notice in the Description page of Project Settings.

#include "MyVRTeleportVisualizer.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"

// Sets default values
AMyVRTeleportVisualizer::AMyVRTeleportVisualizer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	rootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(rootSceneComponent);

	playArea = CreateDefaultSubobject<UNiagaraComponent>(TEXT("playArea"));
	playArea->AttachToComponent(rootSceneComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	ring = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ring"));
	ring->AttachToComponent(rootSceneComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	trace = CreateDefaultSubobject<UNiagaraComponent>(TEXT("trace"));
	trace->AttachToComponent(rootSceneComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// Called when the game starts or when spawned
void AMyVRTeleportVisualizer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyVRTeleportVisualizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyVRTeleportVisualizer::Init(FVector playAreaBounds)
{
	playArea->SetNiagaraVariableVec3("User.PlayAreaBounds", playAreaBounds);
}

void AMyVRTeleportVisualizer::TurnOnTarget()
{
	ring->SetVisibility(true);
	playArea->SetVisibility(true);
}

void AMyVRTeleportVisualizer::TurnOffTarget()
{
	ring->SetVisibility(false);
	playArea->SetVisibility(false);
}

void AMyVRTeleportVisualizer::UpdateTarget(FVector projectedTeleportLocation, FVector relativePlayAreaLocation, FRotator relativePlayAreaRotation)
{
	FHitResult* hitResult = new FHitResult();

	SetActorLocation(projectedTeleportLocation, false, hitResult, ETeleportType::None);

	playArea->SetRelativeLocationAndRotation(relativePlayAreaLocation, relativePlayAreaRotation);
}

void AMyVRTeleportVisualizer::UpdateTrace(TArray<FVector> tracePath, FVector traceLocation)
{
	trace->SetWorldLocation(traceLocation);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(trace, FName("User.PointArray"), tracePath);
}

