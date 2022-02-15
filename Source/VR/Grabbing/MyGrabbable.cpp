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
}

void UMyGrabbable::OnGrab(UMotionControllerComponent* MotionController)
{
	MotionControllerRef = MotionController;

	OnGrabbed.Broadcast();
}

void UMyGrabbable::OnRelease()
{
	MotionControllerRef = nullptr;

	OnDropped.Broadcast();
}