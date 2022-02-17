// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Triggerable.h"
#include "MyBasePistol.generated.h"

class UBaseShooting;
class UMyGrabbable;
class USkeletalMeshComponent;

UCLASS()
class VR_API AMyBasePistol : public AActor, public ITriggerable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyBasePistol();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Trigger();
	bool CanTrigger();

public:
	UPROPERTY(VisibleAnywhere)
		UBaseShooting* shooting;

	UPROPERTY(VisibleAnywhere)
		UMyGrabbable* grabComponent;

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* skeletalMesh;

	UPROPERTY(EditAnywhere)
		USceneComponent* grabPoint;
};
