// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyVRTeleportVisualizer.generated.h"

class UNiagaraComponent;
class UMotionControllerComponent;

UCLASS()
class VR_API AMyVRTeleportVisualizer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyVRTeleportVisualizer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(FVector playAreaBounds);

	void TurnOnTarget();
	void TurnOffTarget();
	void UpdateTarget(FVector projectedTeleportLocation, FVector relativePlayAreaLocation, FRotator relativePlayAreaRotation);
	void UpdateTrace(TArray<FVector> tracePath, FVector traceLocation);

private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* rootSceneComponent;
	UPROPERTY(VisibleAnywhere)
		UNiagaraComponent* playArea;
	UPROPERTY(VisibleAnywhere)
		UNiagaraComponent* ring;
	UPROPERTY(VisibleAnywhere)
		UNiagaraComponent* trace;
};