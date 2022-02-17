// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBasePistol.h"
#include "../Grabbing/MyGrabbable.h"
#include "Components/SkeletalMeshComponent.h"
#include "BaseShooting.h"

// Sets default values
AMyBasePistol::AMyBasePistol()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SetRootComponent(skeletalMesh);

	grabPoint = CreateDefaultSubobject<USceneComponent>("Grab Point");
	grabPoint->SetupAttachment(skeletalMesh);

	shooting = CreateDefaultSubobject<UBaseShooting>("BaseShooting");
	AddOwnedComponent(shooting);

	grabComponent = CreateDefaultSubobject<UMyGrabbable>("GrabComponent");
	AddOwnedComponent(grabComponent);
	grabComponent->grabPoint = grabPoint;
}

// Called when the game starts or when spawned
void AMyBasePistol::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyBasePistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyBasePistol::Trigger()
{
	shooting->Shot();
}

bool AMyBasePistol::CanTrigger()
{
	return true;
}
