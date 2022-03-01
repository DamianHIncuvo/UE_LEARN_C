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
class AMyVRPawn;
class AMyBaseMenu;
class AMyBaseMenu;

/**
 * 
 */
UCLASS()
class VR_API AMyVRPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AMyBaseMenu> menuClass;

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:
	void ToggleMenu(bool isRightHand, AMyVRHand* mainHand, AMyVRHand* otherHand);
	void LeftToggleMenu();
	void RightToggleMenu();

	AMyVRPawn* pawn;
	AMyBaseMenu* menu;

	UTeleportComponent* teleportComponent;
	URotateComponent* rotateComponent;
};