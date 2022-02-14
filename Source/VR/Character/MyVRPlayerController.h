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

private:
	void OnRotateAxis(float inputValue);
	void OnTeleportAxis(float inputValue);
	void OnLeftGrabInput();
	void OnLeftReleaseInput();
	void OnRightGrabInput();
	void OnRightReleaseInput();

	InputRotateProcessor* rotateProcessor;
	InputTeleportProcessor* teleportProcessor;

	UTeleportComponent* teleportComponent;
	URotateComponent* rotateComponent;

	AMyVRHand* leftHand;
	AMyVRHand* rightHand;
};