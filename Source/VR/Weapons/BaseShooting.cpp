// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseShooting.h"

// Sets default values for this component's properties
UBaseShooting::UBaseShooting()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBaseShooting::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBaseShooting::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBaseShooting::Shot()
{
	UE_LOG(LogTemp, Error, TEXT("BAM"));
}

