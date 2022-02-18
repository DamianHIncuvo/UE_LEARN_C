// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyVRProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class VR_API AMyVRProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyVRProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OnProjectileStop(FHitResult& impactResult);

public:
	UPROPERTY(VisibleAnywhere)
		UProjectileMovementComponent* projectileMovement;

	UPROPERTY(VisibleAnywhere)
		USphereComponent* sphereCollision;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* staticMesh;
};
