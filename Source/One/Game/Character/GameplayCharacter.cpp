// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "One/Game/Library/GameplayLibrary.h"

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
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem) return;
	
	Subsystem->ClearAllMappings();  
	Subsystem->AddMappingContext(InputMapping, 0);
	auto* EnhancedInput{ Cast<UEnhancedInputComponent>(PlayerInputComponent) };
	if (IsValid(EnhancedInput))
	{
		EnhancedInput->BindAction(LookMouseAction,	ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse);
		EnhancedInput->BindAction(MoveAction,		ETriggerEvent::Triggered, this, &ThisClass::Input_OnMove);
		EnhancedInput->BindAction(InteractAction,	ETriggerEvent::Triggered, this, &ThisClass::Input_Interact);
		EnhancedInput->BindAction(Action_AIM, 		ETriggerEvent::Triggered, this, &ThisClass::Input_AIM);
		EnhancedInput->BindAction(UseSlot_01, 		ETriggerEvent::Triggered, this, &ThisClass::Input_UseSlot_01);
		EnhancedInput->BindAction(UseSlot_02, 		ETriggerEvent::Triggered, this, &ThisClass::Input_UseSlot_02);
	}
}


void AGameplayCharacter::Input_LookMouse(const FInputActionValue& ActionValue)
{
	const auto Value{ActionValue.Get<FVector2D>()};

	AddControllerYawInput(Value.X * MouseSensitivity.X);
	AddControllerPitchInput(Value.Y * MouseSensitivity.Y);
}

void AGameplayCharacter::Input_AIM(const FInputActionValue& ActionValue)
{
	const auto Value{ActionValue.Get<bool>()};
}
void AGameplayCharacter::Input_Interact(const FInputActionValue& ActionValue)
{
	const auto Value{ActionValue.Get<bool>()};
	if (Value)
	{
		InteractManager->InteractPressed();
	}
	else
	{
		InteractManager->InteractReleased();
	}
}


void AGameplayCharacter::Input_OnMove(const FInputActionValue& ActionValue)
{
	const FVector2D Value{ UGameplayLibrary::ClampMagnitude012D(ActionValue.Get<FVector2D>()) };

	const FVector ForwardDirection{ UGameplayLibrary::AngleToDirectionXY(UE_REAL_TO_FLOAT(GetActorRotation().Yaw)) };
	const FVector RightDirection{ UGameplayLibrary::PerpendicularCounterClockwiseXY(ForwardDirection) };

	AddMovementInput(ForwardDirection * Value.Y + RightDirection * Value.X);
}


void AGameplayCharacter::Input_UseSlot_01(const FInputActionValue& ActionValue)
{
}

void AGameplayCharacter::Input_UseSlot_02(const FInputActionValue& ActionValue)
{
}