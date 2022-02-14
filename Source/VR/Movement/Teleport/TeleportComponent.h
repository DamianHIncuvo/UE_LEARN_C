// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TeleportComponent.generated.h"

class UMotionControllerComponent;
class AMyVRTeleportVisualizer;
class AMyVRPawn;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VR_API UTeleportComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTeleportComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartTeleportTrace();
	void TeleportTrace();
	void EndTeleportTrace();

	void Teleport();
	bool CanTeleport();

public:
	USceneComponent* actorRootSceneComponent;
	UMotionControllerComponent* motionController;
	AMyVRPawn* pawn;

	UPROPERTY(EditAnywhere)
		float teleportProjectileRadius = 3.6f;
	UPROPERTY(EditAnywhere)
		float teleportLaunchSpeed = 650.0f;
	UPROPERTY(EditAnywhere)
		FVector TeleportProjectPointToNavigationQueryExtent = FVector::OneVector;
	UPROPERTY(EditAnywhere)
		float navMeshCellHeight = 8.0f;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AMyVRTeleportVisualizer> teleportVisualizerClass;

private:
	AMyVRTeleportVisualizer* teleportVisualizer;
	bool isVisible = false;
	bool validTeleportLocatioon = false;
	FVector projectedTeleportLocation;
};
