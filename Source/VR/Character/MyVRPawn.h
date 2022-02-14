// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Enums/AxisSide.h"
#include "MyVRPawn.generated.h"

class UCameraComponent;
class UTeleportComponent;
class URotateComponent;
class UMotionControllerComponent;
class AMyVRHand;
class AMyVRPlayerController;

UCLASS()
class VR_API AMyVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyVRPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* sceneRootPlayerRoom;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* camera;

	UPROPERTY(VisibleAnywhere)
		UTeleportComponent* teleportComponent;
	UPROPERTY(VisibleAnywhere)
		URotateComponent* rotateComponent;

	UPROPERTY(VisibleAnywhere)
		AMyVRHand* leftHand;
	UPROPERTY(VisibleAnywhere)
		AMyVRHand* rightHand;

private:
	AMyVRPlayerController* playerController;
};
