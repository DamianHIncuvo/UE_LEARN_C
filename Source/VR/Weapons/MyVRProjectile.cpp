// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/ProjectileMovementComponent.h"
#include "MyVRProjectile.h"

// Sets default values
AMyVRProjectile::AMyVRProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");
	AddOwnedComponent(projectileMovement);
}

// Called when the game starts or when spawned
void AMyVRProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyVRProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

