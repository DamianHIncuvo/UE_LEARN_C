// Fill out your copyright notice in the Description page of Project Settings.


#include "MyVRHand.h"
#include "MotionControllerComponent.h"

// Sets default values
AMyVRHand::AMyVRHand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(root);

	motionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	motionController->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AMyVRHand::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyVRHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

