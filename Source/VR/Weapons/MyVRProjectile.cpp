// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MyVRProjectile.h"

// Sets default values
AMyVRProjectile::AMyVRProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");
	AddOwnedComponent(projectileMovement);

	sphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SetRootComponent(sphereCollision);

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	staticMesh->SetupAttachment(sphereCollision);
}

// Called when the game starts or when spawned
void AMyVRProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, FName("OnProjectileStop"));
	projectileMovement->OnProjectileStop.Add(Delegate);
}

void AMyVRProjectile::OnProjectileStop(FHitResult& impactResult)
{
	UE_LOG(LogTemp, Error, TEXT("WWWWWWWWWWWWWWWWWWWWWWWWWWWWW lel"));
	Destroy();
}

// Called every frame
void AMyVRProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyVRProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (OtherComp->IsSimulatingPhysics() == false)
	//	return;

	//OtherComp->AddImpulse(FVector(GetVelocity()), FName("None"), false);

	//Destroy();
}

//void AMyVRProjectile::OnProjectileStop(FHitResult& impactResult)
//{
//	UE_LOG(LogTemp, Error, TEXT("WWWWWWWWWWWWWWWWWWWWWWWWWWWWW lel"));
//	Destroy();
//}

