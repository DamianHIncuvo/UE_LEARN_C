// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Enums/MyGrabType.h"
#include "MyGrabbable.generated.h"

class UMotionControllerComponent;
class UMyGrabber;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGrabbed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDropped);

UCLASS()
class VR_API UMyGrabbable : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyGrabbable();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		void OnGrab(UMyGrabber* grabberRef, UMotionControllerComponent* MotionController);
	UFUNCTION(BlueprintCallable)
		void OnRelease();

	bool IsHolded();

	UPROPERTY(BlueprintAssignable)
		FGrabbed OnGrabbed;

	UPROPERTY(BlueprintAssignable)
		FDropped OnDropped;

public:
	UPROPERTY(EditAnywhere)
		TEnumAsByte<EMyGrabType> GrabType;

	UPROPERTY(EditAnywhere)
		USceneComponent* grabPoint;

	UMyGrabber* grabberRef;

private:
	UMotionControllerComponent* MotionControllerRef;
};