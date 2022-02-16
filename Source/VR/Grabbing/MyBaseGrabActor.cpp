// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBaseGrabActor.h"
#include "../Grabbing/MyGrabbable.h"

// Sets default values
AMyBaseGrabActor::AMyBaseGrabActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(staticMesh);
}

// Called when the game starts or when spawned
void AMyBaseGrabActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyBaseGrabActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

