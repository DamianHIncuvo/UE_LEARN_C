// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBaseGrabActor.h"

// Sets default values
AMyBaseGrabActor::AMyBaseGrabActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));
	SetRootComponent(staticMesh);

	staticMesh->SetCollisionProfileName("PhysicsActor");
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

