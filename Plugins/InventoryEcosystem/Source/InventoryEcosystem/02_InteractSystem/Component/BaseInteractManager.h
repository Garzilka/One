// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractComponent.h"
#include "Camera/CameraComponent.h"
#include "InventoryEcosystem/01_Core/Component/BaseInventoryComponent.h"
#include "BaseInteractManager.generated.h"



USTRUCT(BlueprintType)
struct FInteractManagerState
{
	GENERATED_BODY()

public:

	void SetNewInteractComponent(UBaseInteractComponent* NewInteractComponent)
	{
		LastInteractComponent = CurrentInteractComponent;
		CurrentInteractComponent = NewInteractComponent;
	}

	UBaseInteractComponent* GetCurrentInteractComponent() const
	{
		return CurrentInteractComponent;
	}

	UBaseInteractComponent* GetLastInteractComponent() const
	{
		return LastInteractComponent;
	}

	bool CanBeTrace() const
	{
		return true;
	}
protected:
	UPROPERTY()
	//Текущий объект взаимодействия
	UBaseInteractComponent* CurrentInteractComponent = nullptr;

	UPROPERTY()
	//Текущий объект взаимодействия
	UBaseInteractComponent* LastInteractComponent = nullptr;

};



static TAutoConsoleVariable<bool> CDrawDebugInteractedLine(TEXT("IES.Interact.ShowInteractTrace"), false, TEXT("Enable draw debug interact"), ECVF_Cheat);

UCLASS()
class INTERACTSYSTEM_API UBaseInteractManager : public UBaseInventoryComponent
{
	GENERATED_BODY()


	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Настройки|Менеджер взаимодействие|Базовые",
		meta = (ClampMin = "1", ClampMax = "100", DisplayName = "Frequence", ForceUnits = "Hz"))
	float InteractionCheckFrequency = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Настройки|Менеджер взаимодействие|Базовые",	meta = (ClampMin = "100"))
	float MaxTraceDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Настройки|Менеджер взаимодействие|Базовые")
	FName InteractProfile = "Interacted";

	UPROPERTY(EditDefaultsOnly, Category = "Настройки|Менеджер взаимодействие|Базовые", meta = (ClampMin = "30"))
	float InteractionSphereSize = 30.f;
	
	// Tick for trace
	UPROPERTY()
	FTimerHandle Timer_InteractTrace;

	UPROPERTY()
	FInteractManagerState InteractManagerState;

	//A timer that checks how long the button has been pressed, and also implements the mechanics of automatic interaction at the end of the timer.
	UPROPERTY()
	FTimerHandle Timer_InteractPressed;

private:
	//Кешированая ссылка на камеру, для избежания лишних кастов
	UPROPERTY()
	UCameraComponent* PlayerCameraComponent;
public:	
	UFUNCTION(BlueprintPure)
	UBaseInteractComponent* GetCurrentInteractComponent() const {	return InteractManagerState.GetCurrentInteractComponent();	};
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void PerformTrace();
	
	
	/** Возвращает позицию удара трейса, по каналу Visible*/
	UFUNCTION(BlueprintPure)
	bool DoCollisionTestHit(FHitResult& OutHitResult, float AppendVector = 0.f);
	
	/** Возвращает позицию удара трейса, по каналу Visible*/
	UFUNCTION(BlueprintPure)
	FVector DoCollisionTest(float AppendVector = 0.f);
	
	/** Взять текущую позицию камеры*/
	FVector GetCameraLocation();
	
	/** Взять направление камеры*/
	FVector GetCameraForwardVector();
	
	/** Положение камеры + (Направление камеры * Дистанция проверки) */
	FVector GetEndVector();
		
	UCameraComponent* GetCameraComponent();
	
	/** Состояние: Сбросить интеракт */
	void InteractionNone();
	
	/*
	* Состояние: Новый интеракт
	* @InteractComponent - новый компонент взаимодействия
	*/
	void NewInteraction(UBaseInteractComponent* InteractComponent);
	
	UBaseInteractComponent* GetInteractionComponent(TArray<FHitResult> Hits, FVector HitLocation, bool IgnorePriority = false);	
};
