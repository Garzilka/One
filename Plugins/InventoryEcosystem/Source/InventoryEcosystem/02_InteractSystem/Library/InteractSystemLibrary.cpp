// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractSystemLibrary.h"

#include "GameFramework/Character.h"
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
