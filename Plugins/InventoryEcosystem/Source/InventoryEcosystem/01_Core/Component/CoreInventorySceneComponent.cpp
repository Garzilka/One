// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreInventorySceneComponent.h"



#pragma region ENGINE

UCoreInventorySceneComponent::UCoreInventorySceneComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


#pragma endregion ENGINE

#pragma region TOOLS

bool UCoreInventorySceneComponent::IsLocallyControlled()
{
	return (GetOwningPlayerCharacter() && GetOwningPlayerCharacter()->IsLocallyControlled());
}

bool UCoreInventorySceneComponent::IsLocallySpectator()
{
	if (!IsValid(GetWorld())) return false;
	APlayerController* LPC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(LPC)) return false;

	if (LPC->IsInState(NAME_Spectating))
	{
		return (LPC->GetViewTarget() == GetOwningPlayerCharacter());
	}
	return false;
}

ACharacter* UCoreInventorySceneComponent::GetOwningPlayerCharacter()
{
	if (IsValid(CharacterOwner)) return CharacterOwner;

	CharacterOwner = Cast<ACharacter>(GetOwner());
	if (!CharacterOwner && GetOwningPlayerController())
	{
		CharacterOwner = Cast<ACharacter>(GetOwningPlayerController()->GetPawn());
	}
	return CharacterOwner;
}

APlayerController* UCoreInventorySceneComponent::GetOwningPlayerController()
{
	if (IsValid(ControllerOwner)) return ControllerOwner;

	ControllerOwner = Cast<APlayerController>(GetOwner());

	if (!ControllerOwner && GetOwningPlayerCharacter())
	{
		ControllerOwner = Cast<APlayerController>(GetOwningPlayerCharacter()->GetController());
	}

	return ControllerOwner;
}

void UCoreInventorySceneComponent::ChangeObjectOuter(UObject* Object, AActor* InNewOuter)
{
	if (!(Object && InNewOuter && InNewOuter->HasAuthority())) return;
	if (InNewOuter == Object->GetOuter()) return;
	Object->Rename(nullptr, InNewOuter);
}

#pragma endregion TOOLS