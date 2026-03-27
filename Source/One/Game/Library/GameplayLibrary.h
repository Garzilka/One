// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "One/Game/HUD/GameplayHUD.h"
#include "GameplayLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ONE_API UGameplayLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "One|GameplayLibrary|UI", meta = (WorldContext = "WorldContextObject"))
	static AGameplayHUD* GetGameplayHUD(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "One|GameplayLibrary|UI", meta = (WorldContext = "WorldContextObject"))
	static void SetUIState(const UObject* WorldContextObject, EUIState NewState);
	
	UFUNCTION(BlueprintPure, Category = "One|GameplayLibrary|Math", Meta = (ReturnDisplayName = "Direction"))
	static FVector AngleToDirectionXY(float Angle);
	
	UFUNCTION(BlueprintPure, Category = "One|GameplayLibrary|Math", Meta = (ReturnDisplayName = "Direction"))
	static FVector RadianToDirectionXY(const float Radian);

	UFUNCTION(BlueprintPure, Category = "One|GameplayLibrary|Math", DisplayName = "Clamp Magnitude 01 2D",
		Meta = (AutoCreateRefTerm = "Vector", ReturnDisplayName = "Vector"))
	static FVector2D ClampMagnitude012D(const FVector2D& Vector);

	UFUNCTION(BlueprintPure, Category = "One|GameplayLibrary|Math", Meta = (AutoCreateRefTerm = "Vector", ReturnDisplayName = "Vector"))
	static FVector PerpendicularCounterClockwiseXY(const FVector& Vector);
};

inline FVector UGameplayLibrary::RadianToDirectionXY(const float Radian)
{
	float Sin, Cos;
	FMath::SinCos(&Sin, &Cos, Radian);

	return { Cos, Sin, 0.0f };
}

inline FVector UGameplayLibrary::AngleToDirectionXY(const float Angle)
{
	return RadianToDirectionXY(FMath::DegreesToRadians(Angle));
}

inline FVector2D UGameplayLibrary::ClampMagnitude012D(const FVector2D& Vector)
{
	const auto MagnitudeSquared{ Vector.SizeSquared() };

	if (MagnitudeSquared <= 1.0f)
	{
		return Vector;
	}

	const auto Scale{ FMath::InvSqrt(MagnitudeSquared) };

	return { Vector.X * Scale, Vector.Y * Scale };
}

inline FVector UGameplayLibrary::PerpendicularCounterClockwiseXY(const FVector& Vector)
{
	return { -Vector.Y, Vector.X, Vector.Z };
}