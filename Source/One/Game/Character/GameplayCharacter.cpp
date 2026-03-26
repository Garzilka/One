// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCharacter.h"

AGameplayCharacter::AGameplayCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	InteractManager = CreateDefaultSubobject<UBaseInteractManager>(TEXT("InteractManager"));
}

void AGameplayCharacter::BeginPlay()
{
	Super::BeginPlay();	
}

void AGameplayCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameplayCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

