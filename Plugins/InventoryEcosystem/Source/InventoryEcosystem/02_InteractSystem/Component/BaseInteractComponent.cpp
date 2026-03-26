// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInteractComponent.h"

void UBaseInteractComponent::BeginFocus(ACharacter* CharacterInstigator)
{
	OnBeginFocus.Broadcast(CharacterInstigator, this);
}

void UBaseInteractComponent::EndFocus(ACharacter* CharacterInstigator)
{
	OnBeginFocus.Broadcast(CharacterInstigator, this);
}

void UBaseInteractComponent::Interact(ACharacter* CharacterInstigator, EInteractType InteractType)
{
	if (!CanInteractFor(CharacterInstigator)) return;

	CurrentInteractTime = 0.f;

	if (InteractType != EInteractType::EIT_CirculPressed)
		InInteract = false;

	OnInteracted.Broadcast(CharacterInstigator, this, InteractType);
}

void UBaseInteractComponent::StartInteract(ACharacter* CharacterInstigator)
{
	if (!CanInteractFor(CharacterInstigator)) return;
	InInteract = CanBePressed();
	OnBeginInteract.Broadcast(CharacterInstigator, this);

	if (!IsNetMode(ENetMode::NM_DedicatedServer))
	{
		TArray<UPrimitiveComponent*> Primitives;
		GetOwner()->GetComponents<UPrimitiveComponent>(Primitives);
		for (auto VisualComp : Primitives)
		{
			if (IsOutLine(VisualComp))
				VisualComp->SetRenderCustomDepth(true);
		}
	}
}

void UBaseInteractComponent::EndInteract(ACharacter* CharacterInstigator)
{
	InInteract = false;
	CurrentInteractTime = (InteractSettings.TypeTimeDegradation == EInteractTypeTimeDegradation::EITTD_Clear ? 0.f : CurrentInteractTime);
	OnEndInteract.Broadcast(CharacterInstigator, this);

	if (!IsNetMode(ENetMode::NM_DedicatedServer))
	{
		TArray<UPrimitiveComponent*> Primitives;
		GetOwner()->GetComponents<UPrimitiveComponent>(Primitives);
		for (auto VisualComp : Primitives)
		{
			if (IsOutLine(VisualComp))
				VisualComp->SetRenderCustomDepth(false);
		}
	}
}

bool UBaseInteractComponent::IsOutLine(UPrimitiveComponent* Primitive)
{
	if (!IsValid(Primitive)) return false;
	return Primitive->ComponentTags.Contains("OutLine");
}

bool UBaseInteractComponent::CanInteractFor(ACharacter* CharacterInstigator)
{
	return IsActive();
}

bool UBaseInteractComponent::HasInteractFlag(EInteractType Type)
{
	return (((uint8)InteractSettings.ActionMenu) & ((uint8)Type));
}

bool UBaseInteractComponent::CanBePressed()
{
	return (HasInteractFlag(EInteractType::EIT_Pressed) || HasInteractFlag(EInteractType::EIT_Released) || HasInteractFlag(EInteractType::EIT_CirculPressed));
}
