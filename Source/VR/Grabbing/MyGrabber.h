// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyGrabber.generated.h"

class AMyVRHand;
class UMotionControllerComponent;
class UMyGrabbable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VR_API UMyGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InputGrab();
	void InputRelease();

	bool IsHoldingGrabbable();

public:
	UPROPERTY(EditAnywhere)
		UHapticFeedbackEffect_Base* OnGrabHapticEffect;

	UMyGrabbable* heldGrabbable;

private:
	AMyVRHand* hand;

	void FreeGrab(UMyGrabbable* grabbable);
	void SnapGrab(UMyGrabbable* grabbable);

	void SetPrimitiveCompPhysics(bool bSimulate, UMyGrabbable* grabbable);

	void AttachParentToMotionController(UMyGrabbable* grabbable);

	EControllerHand GetHandFromMotionSource(UMotionControllerComponent* MotionController);

	void Release();
};
