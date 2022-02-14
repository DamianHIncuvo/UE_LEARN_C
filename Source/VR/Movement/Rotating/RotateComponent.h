// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Enums/AxisSide.h"
#include "RotateComponent.generated.h"

class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VR_API URotateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URotateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SnapTurn(EAxisSide axisSide);

public:
	UPROPERTY(EditAnywhere)
		float snapTurnDegrees = 30.0f;

public:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* actorRootSceneComponent;
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* camera;
};
