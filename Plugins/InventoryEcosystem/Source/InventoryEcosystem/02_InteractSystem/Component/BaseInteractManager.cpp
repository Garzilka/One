// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInteractManager.h"
#include "InventoryEcosystem/02_InteractSystem/Library/InteractSystemLibrary.h"

#pragma region ENGINE

void UBaseInteractManager::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocallyControlled()) return;
	
	GetWorld()->GetTimerManager().SetTimer(Timer_InteractTrace, this, &UBaseInteractManager::PerformTrace, (1.f / InteractionCheckFrequency), true);	
}

#pragma endregion ENGINE

#pragma region TRACE

void UBaseInteractManager::PerformTrace()
{
	if (!InteractManagerState.CanBeTrace()) return;

	TArray<FHitResult> LHitResults;
	FInteractDebugData DrawDebugData = FInteractDebugData::MakeDefault(false);
	FInteractTraceData TraceData(InteractProfile, InteractionSphereSize, GetCameraLocation(), DoCollisionTest(((MaxTraceDistance / 100) * 5)), GetOwningPlayerCharacter());
	
#if !UE_BUILD_SHIPPING
	DrawDebugData = FInteractDebugData::MakeDefault(CDrawDebugInteractedLine.GetValueOnGameThread());
#endif

	//Сначало проверить перед собой, иначе объемный трейс может не дать взаимодействовать при сложной геометрии 
	if (UInteractSystemLibrary::LineTrace(TraceData, LHitResults, DrawDebugData))
	{
		if (UBaseInteractComponent* LTargetInteractComponent = GetInteractionComponent(LHitResults, TraceData.EndVector))
		{
			NewInteraction(LTargetInteractComponent);
			return;
		}
	}
	
	LHitResults.Reset();
	if (!UInteractSystemLibrary::PerformTrace(TraceData, LHitResults, DrawDebugData))
	{
		InteractionNone();
		return;
	}	
	
	if (UBaseInteractComponent* LTargetInteractComponent = GetInteractionComponent(LHitResults, TraceData.EndVector))
	{
		NewInteraction(LTargetInteractComponent);
		return;
	}
	
	InteractionNone();
}

#pragma endregion TRACE

#pragma region INTERACT

UBaseInteractComponent* UBaseInteractManager::GetInteractionComponent(TArray<FHitResult> Hits, FVector HitLocation,	bool IgnorePriority)
{
	float LDistance = 1000.f;
	UBaseInteractComponent* LPossibleInteractComponent = nullptr;

	TMap<UBaseInteractComponent*, FHitResult> Input;
	for (auto Hit : Hits)
	{
		AActor* TargetActor = Hit.GetActor();
		if (!IsValid(TargetActor)) continue;
		
		//Дистанция больше максимальной
		float Distance = FVector::Distance(GetOwningPlayerCharacter()->GetActorLocation(), TargetActor->GetActorLocation());
		if (Distance > MaxTraceDistance) continue;
	
		TArray<UBaseInteractComponent*> LInteractComponents;
		TargetActor->GetComponents<UBaseInteractComponent>(LInteractComponents);
	
		for (auto LInteractComponent : LInteractComponents)
		{
			if (!IsValid(LInteractComponent)) continue;
	
			Distance = FVector::Distance(GetOwningPlayerCharacter()->GetActorLocation(), LInteractComponent->GetComponentLocation());
			if (Distance > LInteractComponent->GetInteractSettings().InteractionDistance) continue;
	
			Input.Add(LInteractComponent, Hit);
		}
	}
	
	for (auto Pair : Input)
	{
		if (!LPossibleInteractComponent)
		{
			LPossibleInteractComponent = Pair.Key;
			LDistance = FVector::Distance(LPossibleInteractComponent->GetOwner()->GetActorLocation(), HitLocation);
			continue;
		}
		
		if (FVector::Distance(Pair.Key->GetOwner()->GetActorLocation(), HitLocation) < LDistance) // New = nearest ->  NewPossible
		{
			LDistance = FVector::Distance(Pair.Value.Location, HitLocation);
	
			LPossibleInteractComponent = Pair.Key;
		}
	}
	return LPossibleInteractComponent;
}


#pragma endregion INTERACT

#pragma region INTERACT_STATE

void UBaseInteractManager::InteractionNone()
{
	if (!GetCurrentInteractComponent()) return;
	
	//Потенциально у нас может смениться LastInteractComponent при вызове OnInteractFocusLost или EndInteract, поэтому тут кешируется тот компонент с которым надо закончить работу
	UBaseInteractComponent* LFinishInteractComponent = GetCurrentInteractComponent();
	InteractManagerState.SetNewInteractComponent(nullptr);

	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(Timer_InteractPressed))
		GetWorld()->GetTimerManager().ClearTimer(Timer_InteractPressed);

	LFinishInteractComponent->EndInteract(GetOwningPlayerCharacter());
	LFinishInteractComponent->EndFocus(GetOwningPlayerCharacter());
}

void UBaseInteractManager::NewInteraction(UBaseInteractComponent* InteractComponent)
{
	if (!IsValid(InteractComponent)) return;
	if (GetCurrentInteractComponent() == InteractComponent) return;
	
	InteractionNone();
	InteractManagerState.SetNewInteractComponent(InteractComponent);
	
	if (!GetCurrentInteractComponent()) return;

	GetCurrentInteractComponent()->BeginFocus(GetOwningPlayerCharacter());
}

#pragma endregion INTERACT_STATE

#pragma region TOOLS
UCameraComponent* UBaseInteractManager::GetCameraComponent()
{
	if (PlayerCameraComponent) return PlayerCameraComponent;
	if (!GetOwningPlayerCharacter()) return nullptr;
	
	PlayerCameraComponent = Cast<UCameraComponent>(GetOwningPlayerCharacter()->GetComponentByClass(UCameraComponent::StaticClass()));
	return PlayerCameraComponent;
}

FVector UBaseInteractManager::GetCameraLocation()
{
	if (GetOwningPlayerController() && GetOwningPlayerController()->PlayerCameraManager)
	{
		return GetOwningPlayerController()->PlayerCameraManager->GetCameraLocation();
	}
	else if (GetCameraComponent())
	{
		return GetCameraComponent()->GetComponentLocation();
	}
	return FVector::ZeroVector;
}

FVector UBaseInteractManager::GetCameraForwardVector()
{
	if (GetOwningPlayerController() && GetOwningPlayerController()->PlayerCameraManager)
	{
		return GetOwningPlayerController()->PlayerCameraManager->GetCameraRotation().Vector();
	}
	else 
		if (GetCameraComponent())
		{
			return GetCameraComponent()->GetForwardVector();
		}
	return FVector::ZeroVector;
}

FVector UBaseInteractManager::GetEndVector()
{
	return (GetCameraLocation() + GetCameraForwardVector() * MaxTraceDistance);
}

bool UBaseInteractManager::DoCollisionTestHit(FHitResult& OutHitResult, float AppendVector)
{
	FCollisionQueryParams LCQP;
	LCQP.AddIgnoredActor(GetOwningPlayerCharacter());
	return GetWorld()->LineTraceSingleByChannel(OutHitResult, GetCameraLocation(), GetEndVector(), ECollisionChannel::ECC_Visibility, LCQP);
}

FVector UBaseInteractManager::DoCollisionTest(float AppendVector)
{
	FHitResult HitResult;
	bool LResult = DoCollisionTestHit(HitResult, AppendVector);
	return (LResult ? HitResult.Location : GetEndVector());
}

#pragma endregion TOOLS