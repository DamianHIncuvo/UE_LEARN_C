// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Enums/MyGrabType.h"
#include "MyGrabbable.generated.h"

class UMotionControllerComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGrabbed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDropped);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
		bool TryGrab(UMotionControllerComponent* MotionController);
	UFUNCTION(BlueprintCallable)
		bool TryRelease();

	UPROPERTY(BlueprintAssignable)
		FGrabbed OnGrabbed;

	UPROPERTY(BlueprintAssignable)
		FDropped OnDropped;

private:

	void TryFreeGrab(UMotionControllerComponent* MotionController);
	void TrySnapGrab(UMotionControllerComponent* MotionController);

	void Release();

	EControllerHand GetHandFromMotionSource(UMotionControllerComponent* MotionController);

	void SetPrimitiveCompPhysics(bool bSimulate);

	bool AttachParentToMotionController(UMotionControllerComponent* MotionController);

public:
	UPROPERTY(EditAnywhere)
		TEnumAsByte<EMyGrabType> GrabType;

	UPROPERTY(EditAnywhere)
		UHapticFeedbackEffect_Base* OnGrabHapticEffect;

private:
	UMotionControllerComponent* MotionControllerRef;

	bool bSimulateOnDrop;
	bool bIsHeld;
};