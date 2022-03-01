// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Enums/MenuAnimationStates.h"
#include "MyBaseMenu.generated.h"

class UWidgetComponent;
class UNiagaraComponent;
class UMotionControllerComponent;
class UWidgetInteractionComponent;
class APlayerCameraManager;

UCLASS()
class VR_API AMyBaseMenu : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyBaseMenu();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ChangeState(EMenuAnimationStates newState);

public: 
	UPROPERTY(EditAnywhere)
		UWidgetComponent* widget;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* cursor;
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* menuLaser;
	UPROPERTY(EditAnywhere)
		USceneComponent* root;
	UPROPERTY(EditAnywhere)
		UCurveFloat* curve;
	UPROPERTY(EditAnywhere)
		float menuDistanceTowardsCamera = 10.0f;
	UPROPERTY(EditAnywhere)
		float cursorSpeed = -8.0f;

	bool isActiveMenuHandRight;

	UPROPERTY(EditAnywhere)
	EMenuAnimationStates animationState = EMenuAnimationStates::Showing;
	UPROPERTY(EditAnywhere)
	float scaleWorking = 0;
	UPROPERTY(EditAnywhere)
		float animationTime = 0;

	UWidgetInteractionComponent* widgetInteractionMain;
	UWidgetInteractionComponent* widgetInteractionOther;
	UMotionControllerComponent* motionControllerRef;

private:
	void SetupMenuInputComponent();
	void TriggerLeftPress();
	void TriggerLeftRelease();
	void TriggerRightPress();
	void TriggerRightRelease();

	float widgetRelativeScaleOriginal;
	float cursorLocationLimitY;
	float cursorLocationLimitZ;
	bool isLaserEnabled = false;
	bool isThumbstickEnabled = true;
	float zAxis;
	float yAxis;
	APlayerCameraManager* cameraManager;
};
