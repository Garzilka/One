// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractSystemLibrary.h"

#include "GameFramework/Character.h"
#include "InventoryEcosystem/02_InteractSystem/Component/BaseInteractComponent.h"
#include "InventoryEcosystem/02_InteractSystem/Component/InteractAnimationComponent.h"
#include "Kismet/KismetSystemLibrary.h"


bool UInteractSystemLibrary::LineTrace(FInteractTraceData TraceData, TArray<FHitResult>& OutPut, FInteractDebugData IsShowDrawDebug)
{
	FHitResult Result;
	if (UKismetSystemLibrary::LineTraceSingleByProfile(
			TraceData.Character,
			TraceData.CameraLocation,
			TraceData.EndVector,
			TraceData.InteractProfile,
			false,
			{TraceData.Character},
			(IsShowDrawDebug.IsEnabled ? EDrawDebugTrace::Type::ForDuration : EDrawDebugTrace::Type::None),
			Result, 
			true))
	{
		OutPut.Add(Result);
	}
	return (OutPut.Num() > 0);
}



bool UInteractSystemLibrary::PerformTrace(FInteractTraceData TraceData, TArray<FHitResult>& OutPut,	FInteractDebugData IsShowDrawDebug)
{
	bool Result = UKismetSystemLibrary::CapsuleTraceMultiByProfile(
		TraceData.Character->GetWorld(),
		TraceData.CameraLocation,
		TraceData.EndVector,
		TraceData.InteractionSphereSize,
		TraceData.InteractionSphereSize,
		TraceData.InteractProfile,
		false,
		{ TraceData.Character },
		(IsShowDrawDebug.IsEnabled ? EDrawDebugTrace::Type::ForDuration : EDrawDebugTrace::Type::None),
		OutPut,
		true);

	return Result;
}

UInteractAnimationComponent* UInteractSystemLibrary::GetInteractAnimationComponent(UBaseInteractComponent* From)
{
	if (!(IsValid(From) && From->GetOwner())) return nullptr;
	if (From->GetInteractAnimationComponent()) return From->GetInteractAnimationComponent();
	TArray<UInteractAnimationComponent*> Components;
	From->GetOwner()->GetComponents<UInteractAnimationComponent>(Components);

	UInteractAnimationComponent* Result = nullptr;
	for (auto Component : Components)
	{
		if (!Result)
		{
			Result = Component;
			continue;
		}
		float Distance_1 = FVector::Distance(From->GetComponentLocation(), Result->GetComponentLocation());
		float Distance_2 = FVector::Distance(From->GetComponentLocation(), Component->GetComponentLocation());
		if (Distance_2 < Distance_1)
		{
			Result = Component;
		}
	}

	return Result;
}

UInteractAnimationComponent* UInteractSystemLibrary::GetInteractAnimationFromActor(AActor* From)
{
	if (!IsValid(From)) return nullptr;
	TArray<UInteractAnimationComponent*> Components;
	From->GetOwner()->GetComponents<UInteractAnimationComponent>(Components);

	for (auto Component : Components)
	{
		return Component;
	}

	return nullptr;
}
