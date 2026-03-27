// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayHUD.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"


AGameplayHUD::AGameplayHUD(){}

void AGameplayHUD::BeginPlay()
{
	if (!GetOwningPlayerController()) return;

	APlayerController* PlayerController = Cast<APlayerController>(GetOwningPlayerController());
	if (!PlayerController) return;
	
	auto* EnhancedInput{ Cast<UEnhancedInputComponent>(GetOwningPlayerController()->InputComponent) };
	
	if (IsValid(EnhancedInput))
	{
		EnhancedInput->BindAction(Action_Menu,	ETriggerEvent::Triggered, this, &ThisClass::OpenMenu);
	}

	if (UKismetSystemLibrary::IsValidClass(GameWidgetClass))
	{
		if (IsValid(GameWidget = CreateWidget(GetWorld(), GameWidgetClass)))
		{
			GameWidget->AddToViewport();
		}
	}
	SetUIState(EUIState::EUI_Game);
}

ACharacter* AGameplayHUD::GetPlayerCharacter()
{
	if (!PlayerCharacter) PlayerCharacter = Cast<ACharacter>(GetOwningPawn());
	return PlayerCharacter;
}

void AGameplayHUD::SetUIState(EUIState NewState)
{
	OldStateType = CurrentStateType;
	CurrentStateType = NewState;
	
	if (CurrentStateType != EUIState::EUI_Game)
	{
		SetMenu();
	}
	else
	{
		SetGame();
	}
	OnChangeUIState.Broadcast(NewState, OldStateType);
}

void AGameplayHUD::OpenMenu()
{
	SetUIState(CurrentStateType == EUIState::EUI_Game ? EUIState::EUI_GameMenu : EUIState::EUI_Game);
}


void AGameplayHUD::SetGame()
{
	if (!GetOwningPlayerController()->bShowMouseCursor) return;
	
	FInputModeGameOnly GameOnly;
	GetOwningPlayerController()->bShowMouseCursor = false;
	GetOwningPlayerController()->SetInputMode(GameOnly);
	GetPlayerCharacter()->EnableInput(GetOwningPlayerController());
	if (GetPlayerCharacter()->GetCharacterMovement()->IsFalling()) return;
	
	GetPlayerCharacter()->GetCharacterMovement()->Velocity = FVector::ZeroVector;	
}

void AGameplayHUD::SetMenu()
{
	if (GetOwningPlayerController()->bShowMouseCursor) return;
	
	FInputModeGameAndUI GameAndUi;
	GetOwningPlayerController()->bShowMouseCursor = true;
	GetOwningPlayerController()->SetInputMode(GameAndUi);
	GetPlayerCharacter()->DisableInput(GetOwningPlayerController());
	if (GetPlayerCharacter()->GetCharacterMovement()->IsFalling()) return;
		
	GetPlayerCharacter()->GetCharacterMovement()->Velocity = FVector::ZeroVector;
}