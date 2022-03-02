// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SceneCaptureComponent2D.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyVRSpectator.h"

// Sets default values
AMyVRSpectator::AMyVRSpectator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(root);

	sceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>("SceneCapture2D");
	sceneCaptureComponent->SetupAttachment(root);

	cameraMesh = CreateDefaultSubobject<UStaticMeshComponent>("CameraMesh");
	cameraMesh->SetupAttachment(sceneCaptureComponent);

	textRender = CreateDefaultSubobject<UTextRenderComponent>("TextRender");
	textRender->SetupAttachment(cameraMesh);
}

// Called when the game starts or when spawned
void AMyVRSpectator::BeginPlay()
{
	Super::BeginPlay();
	
	if (UGameplayStatics::GetPlatformName() == FString("Android"))
	{
		Destroy();
		return;
	}

	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	InitializeFieldOfView();

	if (enableSpectatorOnStart)
		EnableVRSpectator();

	SetupSpectatorInputComponent();

	OnDestroyed.AddDynamic(this, &AMyVRSpectator::OnDestroy);
}

// Called every frame
void AMyVRSpectator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleRotation();
	HandleMovement();
	HandleFieldOfView();
}

void AMyVRSpectator::EnableVRSpectator()
{
	spectatorEnabled = true;
	SetActorTickEnabled(true);
	sceneCaptureComponent->Activate();

	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		UHeadMountedDisplayFunctionLibrary::SetSpectatorScreenMode(ESpectatorScreenMode::Texture);
		UHeadMountedDisplayFunctionLibrary::SetSpectatorScreenTexture(texture);

		UE_LOG(LogTemp, Display, TEXT("Spectator Camera Enabled"));
	}
	else
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(this, 0.0f, EViewTargetBlendFunction::VTBlend_Linear);

		UE_LOG(LogTemp, Display, TEXT("Spectator Camera Debug Mode Enabled: No HMD!"));
	}
}

void AMyVRSpectator::DisableVRSpectator()
{
	spectatorEnabled = false;
	SetActorTickEnabled(false);
	UHeadMountedDisplayFunctionLibrary::SetSpectatorScreenMode(ESpectatorScreenMode::SingleEyeCroppedToFill);
	sceneCaptureComponent->Deactivate();
}

void AMyVRSpectator::SetupSpectatorInputComponent()
{
	InputComponent->BindAxis(FName("SpectatorFOV")).bConsumeInput = true;
	InputComponent->BindAxis(FName("SpectatorYaw")).bConsumeInput = true;
	InputComponent->BindAxis(FName("SpectatorPitch")).bConsumeInput = true;
	InputComponent->BindAxis(FName("SpectatorMoveUp")).bConsumeInput = true;
	InputComponent->BindAxis(FName("SpectatorMoveForward")).bConsumeInput = true;
	InputComponent->BindAxis(FName("SpectatorMoveRight")).bConsumeInput = true;
	InputComponent->BindAction(FName("SpectatorResetFOV"), EInputEvent::IE_Pressed, this, &AMyVRSpectator::ResetFOV).bConsumeInput = true;
	InputComponent->BindAction(FName("SpectatorActiveToggle"), EInputEvent::IE_Pressed, this, &AMyVRSpectator::ToggleSpectator).bConsumeInput = true;
	InputComponent->BindAction(FName("SpectatorResetRotation"), EInputEvent::IE_Pressed, this, &AMyVRSpectator::ResetRotation).bConsumeInput = true;
	InputComponent->BindAction(FName("SpectatorFadeToggle"), EInputEvent::IE_Pressed, this, &AMyVRSpectator::FadeToggle).bConsumeInput = true;
}

void AMyVRSpectator::HandleRotation()
{
	yawRotationDelta = UKismetMathLibrary::FInterpTo(yawRotationDelta, GetInputAxisValue(FName("SpectatorYaw")), UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), rotationInterpSpeed);
	AddActorWorldRotation(FRotator(0.0f, yawRotationDelta, 0.0f));

	pitchRotationDelta = UKismetMathLibrary::FInterpTo(pitchRotationDelta, GetInputAxisValue(FName("SpectatorPitch")), UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), rotationInterpSpeed);

	// Prevent gimbal lock
	if (UKismetMathLibrary::Abs(sceneCaptureComponent->GetRelativeRotation().Pitch + pitchRotationDelta) <= 89.9f)
	{
		sceneCaptureComponent->AddRelativeRotation(FRotator(pitchRotationDelta, 0.0f, 0.0f));
	}
}

void AMyVRSpectator::HandleMovement()
{
	FVector forwardDelta = sceneCaptureComponent->GetForwardVector() * GetInputAxisValue(FName("SpectatorMoveForward"));
	FVector rightDelta = sceneCaptureComponent->GetRightVector() * GetInputAxisValue(FName("SpectatorMoveRight"));
	FVector upDelta = FVector::UpVector * GetInputAxisValue(FName("SpectatorMoveUp"));

	movementOffset = UKismetMathLibrary::VInterpTo(
		movementOffset,
		(forwardDelta + rightDelta + upDelta) * movementSpeedMultiplier,
		UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),
		cameraMovementInterpSpeed
	);

	AddActorWorldOffset(movementOffset);
}

void AMyVRSpectator::HandleFieldOfView()
{
	fOVAngleDelta = UKismetMathLibrary::FInterpTo(fOVAngleDelta, GetInputAxisValue(FName("SpectatorFOV")) * fOVSpeedMultiplier, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), fOVInterpSpeed);

	sceneCaptureComponent->FOVAngle = UKismetMathLibrary::FClamp(sceneCaptureComponent->FOVAngle + fOVAngleDelta, minFOV, maxFOV);
}

void AMyVRSpectator::InitializeFieldOfView()
{
	sceneCaptureComponent->FOVAngle = defaultFieldOfView;
}

void AMyVRSpectator::OnDestroy(AActor* Act)
{

}

void AMyVRSpectator::ResetFOV()
{
	fOVAngleDelta = 0.0f;
	InitializeFieldOfView();
}

void AMyVRSpectator::ToggleSpectator()
{
	if (spectatorEnabled)
	{
		DisableVRSpectator();
	}
	else
	{
		EnableVRSpectator();
	}
}

void AMyVRSpectator::ResetRotation()
{
	sceneCaptureComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
}

void AMyVRSpectator::FadeToggle()
{

}
