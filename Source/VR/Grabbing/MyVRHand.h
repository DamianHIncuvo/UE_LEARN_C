// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyVRHand.generated.h"

class UMotionControllerComponent;
class UMyGrabbable;
class UMyGrabber;
class UMyGrabbablesProvider;

UCLASS()
class VR_API AMyVRHand : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyVRHand();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* root;
	UPROPERTY(VisibleAnywhere)
		UMotionControllerComponent* motionController;
	UPROPERTY(VisibleAnywhere)
		UMyGrabber* grabber;
	UPROPERTY(VisibleAnywhere)
		UMyGrabbablesProvider* grabbablesProvider;
	UPROPERTY(EditAnywhere)
		float GrabRadiusFromGripPosition = 6.0f;
};
