// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInteractComponent.h"

#include "Components/ArrowComponent.h"
#include "InventoryEcosystem/01_Core/Library/InventoryCoreLibrary.h"

UBaseInteractComponent::UBaseInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
	bTickInEditor = true;
}

void UBaseInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

#if WITH_EDITOR
	if (GetWorld() && AngleSettings.bWithAngleCheck && AngleSettings.bEnableDebug)
	{
		if ((GetWorld()->WorldType == EWorldType::Editor && !UInventoryCoreLibrary::IsInGameView()) || GetWorld()->WorldType == EWorldType::EditorPreview)
		{
			int32 NumSec = (int32(AngleSettings.InteractAngle / 10.f) * 2);
		
			FRotator Rot = FRotator(0.f, AngleSettings.InteractAngleOffset, 0.f);
		
			UKismetSystemLibrary::DrawDebugConeInDegrees(GetWorld(), GetComponentLocation(), Rot.RotateVector(GetForwardVector()), InteractSettings.InteractionDistance, (AngleSettings.InteractAngle / 2.0f), 0.f, NumSec, FColor::Blue, 0.f, 0.5f);
			UKismetSystemLibrary::DrawDebugConeInDegrees(GetWorld(), GetComponentLocation(), Rot.RotateVector(GetForwardVector()), InteractSettings.InteractionDistance, 0.f, (AngleSettings.InteractAngle / 2.0f), NumSec, FColor::Blue, 0.f, 0.5f);
			UKismetSystemLibrary::DrawDebugConeInDegrees(GetWorld(), GetComponentLocation(), Rot.RotateVector(GetForwardVector()), InteractSettings.InteractionDistance, (AngleSettings.InteractAngle / 2.0f), (AngleSettings.InteractAngle / 2.0f), NumSec, FColor::Blue, 0.f, 0.5f);
		}
	}
#endif
	
	if (!InInteract)
	{
		if (InteractSettings.TypeTimeDegradation == EInteractTypeTimeDegradation::EITTD_Reverse && CurrentInteractTime > 0.001f)
			CurrentInteractTime -= (DeltaTime * InteractSettings.SpeedOfRevers);
		return;
	}
	
	CurrentInteractTime = (CurrentInteractTime > InteractSettings.InteractionTime ? InteractSettings.InteractionTime : CurrentInteractTime + DeltaTime);
}

void UBaseInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	SetActive(true);
}

void UBaseInteractComponent::BeginFocus(ACharacter* CharacterInstigator)
{
	OnBeginFocus.Broadcast(CharacterInstigator, this, GetOwner());

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

void UBaseInteractComponent::EndFocus(ACharacter* CharacterInstigator)
{
	OnBeginFocus.Broadcast(CharacterInstigator, this, GetOwner());

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
}

void UBaseInteractComponent::EndInteract(ACharacter* CharacterInstigator)
{
	InInteract = false;
	CurrentInteractTime = (InteractSettings.TypeTimeDegradation == EInteractTypeTimeDegradation::EITTD_Clear ? 0.f : CurrentInteractTime);
	OnEndInteract.Broadcast(CharacterInstigator, this);
}

bool UBaseInteractComponent::IsOutLine(UPrimitiveComponent* Primitive)
{
	if (!IsValid(Primitive)) return false;
	return Primitive->ComponentTags.Contains("OutLine");
}

float UBaseInteractComponent::GetTimeToInteract() const
{
	return (InteractSettings.InteractionTime - CurrentInteractTime < 0 ? 0.f : InteractSettings.InteractionTime - CurrentInteractTime);
}

float UBaseInteractComponent::GetInteractPercent() const
{
	if (InteractSettings.InteractionTime > 0)
	{
		return (CurrentInteractTime / InteractSettings.InteractionTime);
	}
	
	return 0.f;
}

bool UBaseInteractComponent::CanInteractFor(ACharacter* CharacterInstigator) const
{
	return IsActive();
}

bool UBaseInteractComponent::HasInteractFlag(EInteractType Type) const
{
	return (((uint8)InteractSettings.ActionMenu) & ((uint8)Type));
}

bool UBaseInteractComponent::CanBePressed() const
{
	return (HasInteractFlag(EInteractType::EIT_Pressed) || HasInteractFlag(EInteractType::EIT_Released) || HasInteractFlag(EInteractType::EIT_CirculPressed));
}
