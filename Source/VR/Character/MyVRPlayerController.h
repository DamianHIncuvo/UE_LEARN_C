// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyVRPlayerController.generated.h"

class InputRotateProcessor;
class InputTeleportProcessor;
class UTeleportComponent;
class URotateComponent;
class AMyVRHand;

/**
 * 
 */
UCLASS()
class VR_API AMyVRPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

public:
	AMyVRHand* leftHand;
	AMyVRHand* rightHand;

private:
	void OnRotateAxis(float inputValue);
	void OnTeleportAxis(float inputValue);
	void OnLeftGrabInput();
	void OnLeftReleaseInput();
	void OnRightGrabInput();
	void OnRightReleaseInput();

	void OnGrabInput(AMyVRHand* hand);

	InputRotateProcessor* rotateProcessor;
	InputTeleportProcessor* teleportProcessor;

	UTeleportComponent* teleportComponent;
	URotateComponent* rotateComponent;
};