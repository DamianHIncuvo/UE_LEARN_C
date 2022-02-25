// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBaseMenu.h"

// Sets default values
AMyBaseMenu::AMyBaseMenu()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyBaseMenu::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyBaseMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

