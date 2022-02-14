// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGrabbablesProvider.h"
#include "../Grabbing/MyGrabbable.h"

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
	
}


// Called every frame
void UMyGrabbablesProvider::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UMyGrabbable* UMyGrabbablesProvider::GetBestGrabbableToGrab()
{
	return nullptr;
}

