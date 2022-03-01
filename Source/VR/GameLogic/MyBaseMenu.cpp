// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBaseMenu.h"
#include "NiagaraComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "MotionControllerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "../Enums/MenuAnimationStates.h"

// Sets default values
AMyBaseMenu::AMyBaseMenu()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(root);

	widget = CreateDefaultSubobject<UWidgetComponent>("Widget");
	widget->SetupAttachment(root);

	cursor = CreateDefaultSubobject<UStaticMeshComponent>("Cursor");
	cursor->SetupAttachment(widget);

	menuLaser = CreateDefaultSubobject<UNiagaraComponent>("MenuLaser");
	menuLaser->SetupAttachment(root);
}

// Called when the game starts or when spawned
void AMyBaseMenu::BeginPlay()
{
	Super::BeginPlay();

	widgetRelativeScaleOriginal = widget->GetRelativeScale3D().X;
	cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	SetupMenuInputComponent();
}

// Called every frame
void AMyBaseMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector motionControllerLocation = motionControllerRef->GetComponentLocation();
	FRotator lookAtCameraRotation = UKismetMathLibrary::FindLookAtRotation(motionControllerLocation, cameraManager->GetCameraLocation());
	widget->SetWorldLocation(
		motionControllerLocation +
		UKismetMathLibrary::GetForwardVector(lookAtCameraRotation) * menuDistanceTowardsCamera +
		UKismetMathLibrary::GetUpVector(lookAtCameraRotation) * menuDistanceTowardsCamera
	);

	widget->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(widget->GetComponentLocation(), cameraManager->GetCameraLocation()));

	if (widgetInteractionOther->IsOverHitTestVisibleWidget())
	{
		if (isLaserEnabled == false)
		{
			isLaserEnabled = true;
			menuLaser->SetVisibility(true);
			widgetInteractionMain->AttachToComponent(motionControllerRef, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}

		FVector ImpactPoint = widgetInteractionOther->GetLastHitResult().ImpactPoint;
		cursor->SetWorldLocation(ImpactPoint);

		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVectorValue(menuLaser, FName("User.PointArray"), 0, widgetInteractionOther->GetComponentLocation(), false);
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVectorValue(menuLaser, FName("User.PointArray"), 1, ImpactPoint, false);
	}
	else
	{
		if (isLaserEnabled)
		{
			isLaserEnabled = false;
			menuLaser->SetVisibility(false);
			widgetInteractionMain->AttachToComponent(cursor, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("None"));
			widgetInteractionMain->SetRelativeLocation(FVector(-10.0f, 0.0f, 0.0f), false, nullptr, ETeleportType::TeleportPhysics);
		}

		zAxis = isActiveMenuHandRight ? GetInputAxisValue(FName("MovementAxisRight_X")) : GetInputAxisValue(FName("RotateAxis"));
		yAxis = isActiveMenuHandRight ? GetInputAxisValue(FName("TeleportAxis")) : GetInputAxisValue(FName("MovementAxisLeft_Y"));

		FVector newLocation = FVector::ZeroVector;
		newLocation.Y = UKismetMathLibrary::Clamp(cursor->GetRelativeLocation().Y + zAxis * cursorSpeed, -cursorLocationLimitY, cursorLocationLimitY);
		newLocation.Z = UKismetMathLibrary::Clamp(cursor->GetRelativeLocation().Z + yAxis * FMath::Abs(cursorSpeed), -cursorLocationLimitZ, cursorLocationLimitZ);

		cursor->SetRelativeLocation(newLocation, false, nullptr, ETeleportType::None);
	}

	switch (animationState)
	{
		case EMenuAnimationStates::Showing:
			animationTime += DeltaTime;
			scaleWorking = curve->GetFloatValue(animationTime) * widgetRelativeScaleOriginal;	
			if (scaleWorking >= widgetRelativeScaleOriginal * 0.9999f)
			{
				scaleWorking = widgetRelativeScaleOriginal;
				ChangeState(EMenuAnimationStates::Idle);
			}
			widget->SetRelativeScale3D(scaleWorking * FVector::OneVector);
			break;
		case EMenuAnimationStates::Hiding:
			animationTime -= DeltaTime;
			scaleWorking = curve->GetFloatValue(animationTime) * widgetRelativeScaleOriginal;
			if (scaleWorking <= 0.0f)
			{
				widgetInteractionMain->AttachToComponent(Cast<USceneComponent>(motionControllerRef), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				Destroy();
			}
			else
			{
				widget->SetRelativeScale3D(scaleWorking * FVector::OneVector);
			}
			break;
	}
}

void AMyBaseMenu::ChangeState(EMenuAnimationStates newState)
{
	FVector2D scale = widget->GetUserWidgetObject()->GetPaintSpaceGeometry().GetAbsoluteSize();

	switch (newState)
	{
		case EMenuAnimationStates::Showing:
			break;
		case EMenuAnimationStates::Idle:
			cursorLocationLimitY = scale.X * 0.5f;
			cursorLocationLimitZ = scale.Y * 0.5f;
			break;
		case EMenuAnimationStates::Hiding:
			break;
	}

	animationState = newState;
}

void AMyBaseMenu::SetupMenuInputComponent()
{
	if (isActiveMenuHandRight)
	{
		InputComponent->BindAxis(FName("MovementAxisRight_X"));
		InputComponent->BindAxis(FName("TeleportAxis"));
	}
	else
	{
		InputComponent->BindAxis(FName("RotateAxis"));
		InputComponent->BindAxis(FName("MovementAxisLeft_Y"));
	}

	InputComponent->BindAction("TriggerLeft", EInputEvent::IE_Pressed, this, &AMyBaseMenu::TriggerLeftPress);
	InputComponent->BindAction("TriggerLeft", EInputEvent::IE_Released, this, &AMyBaseMenu::TriggerLeftRelease);
	InputComponent->BindAction("TriggerRight", EInputEvent::IE_Pressed, this, &AMyBaseMenu::TriggerRightPress);
	InputComponent->BindAction("TriggerRight", EInputEvent::IE_Released, this, &AMyBaseMenu::TriggerRightRelease);
}

void AMyBaseMenu::TriggerLeftPress()
{
	if (isActiveMenuHandRight)
	{
		widgetInteractionOther->PressPointerKey(FKey("LeftMouseButton"));
	}
	else
	{
		widgetInteractionMain->PressPointerKey(FKey("LeftMouseButton"));
	}
}
void AMyBaseMenu::TriggerLeftRelease()
{
	if (isActiveMenuHandRight)
	{
		widgetInteractionOther->ReleasePointerKey(FKey("LeftMouseButton"));
	}
	else
	{
		widgetInteractionMain->ReleasePointerKey(FKey("LeftMouseButton"));
	}
}
void AMyBaseMenu::TriggerRightPress()
{
	if (isActiveMenuHandRight)
	{
		widgetInteractionMain->PressPointerKey(FKey("LeftMouseButton"));
	}
	else
	{
		widgetInteractionOther->PressPointerKey(FKey("LeftMouseButton"));
	}
}
void AMyBaseMenu::TriggerRightRelease()
{
	if (isActiveMenuHandRight)
	{
		widgetInteractionMain->ReleasePointerKey(FKey("LeftMouseButton"));
	}
	else
	{
		widgetInteractionOther->ReleasePointerKey(FKey("LeftMouseButton"));
	}
}