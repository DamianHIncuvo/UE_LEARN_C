// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyVRSpectator.generated.h"

class USceneCaptureComponent2D;
class UTextRenderComponent;

UCLASS()
class VR_API AMyVRSpectator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyVRSpectator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleAnywhere)
		USceneComponent* root;
	UPROPERTY(VisibleAnywhere)
		USceneCaptureComponent2D* sceneCaptureComponent;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* cameraMesh;
	UPROPERTY(VisibleAnywhere)
		UTextRenderComponent* textRender;

	UPROPERTY(EditAnywhere)
		UTexture* texture;
	UPROPERTY(EditAnywhere)
		float defaultFieldOfView = 90.0f;
	UPROPERTY(EditAnywhere)
		bool enableSpectatorOnStart = false;
	UPROPERTY(EditAnywhere)
		float rotationInterpSpeed = 2.0f;
	UPROPERTY(EditAnywhere)
		float cameraMovementInterpSpeed = 2.0f;
	UPROPERTY(EditAnywhere)
		float movementSpeedMultiplier = 1.0f;
	UPROPERTY(EditAnywhere)
		float fOVSpeedMultiplier = -5.0f;
	UPROPERTY(EditAnywhere)
		float fOVInterpSpeed = 3.0f;
	UPROPERTY(EditAnywhere)
		float minFOV = 1.0f;
	UPROPERTY(EditAnywhere)
		float maxFOV = 150.0f;

private:
	void EnableVRSpectator();
	void DisableVRSpectator();
	void SetupSpectatorInputComponent();
	void HandleRotation();
	void HandleMovement();
	void HandleFieldOfView();
	void InitializeFieldOfView();
	void OnDestroy(AActor* Act);
	void ResetFOV();
	void ToggleSpectator();
	void ResetRotation();
	void FadeToggle();

	bool spectatorEnabled = false;
	float yawRotationDelta;
	float pitchRotationDelta;
	float fOVAngleDelta;
	FVector movementOffset = FVector::ZeroVector;
};
