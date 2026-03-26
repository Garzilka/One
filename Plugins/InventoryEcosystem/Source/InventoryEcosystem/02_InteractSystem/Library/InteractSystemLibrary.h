// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InteractSystemLibrary.generated.h"

USTRUCT(BlueprintType)
struct FInteractDebugData
{
	GENERATED_BODY()

public:
	FColor BlockTraceColor;
	FColor TryTraceColor;
	bool IsEnabled = false;
	static FInteractDebugData MakeDefault(bool IsEnabled)
	{
		FInteractDebugData Result;
		Result.TryTraceColor = FColor::Green;
		Result.BlockTraceColor = FColor::Red;
		Result.IsEnabled = IsEnabled;
		return Result;
	}

	static FInteractDebugData MakeWallDetect(bool IsEnabled)
	{
		FInteractDebugData Result;
		Result.TryTraceColor = FColor::Blue;
		Result.BlockTraceColor = FColor::Red;
		Result.IsEnabled = IsEnabled;
		return Result;
	}

};

USTRUCT(BlueprintType)
struct FInteractTraceData
{
	GENERATED_BODY()

public:
	FInteractTraceData() = default;
	FInteractTraceData(FName InInteractProfile, float InInteractionSphereSize, FVector InCameraLocation, FVector InEndVector, ACharacter* InCharacter)
	{
		InteractProfile = InInteractProfile;
		InteractionSphereSize = InInteractionSphereSize;
		CameraLocation = InCameraLocation;
		EndVector = InEndVector;
		Character = InCharacter;
	}
	FName InteractProfile = NAME_None;
	float InteractionSphereSize = 30.f;
	FVector CameraLocation = FVector(0);
	FVector EndVector = FVector::ZeroVector;
	ACharacter* Character = nullptr;
};

UCLASS()
class INTERACTSYSTEM_API UInteractSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static bool LineTrace(FInteractTraceData TraceData, TArray<FHitResult>& OutPut, FInteractDebugData IsShowDrawDebug);
	static bool PerformTrace(FInteractTraceData TraceData, TArray<FHitResult>& OutPut, FInteractDebugData IsShowDrawDebug);
};
