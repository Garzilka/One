// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "BaseInteractComponent.h"
#include "GameplayTagContainer.h"
#include "InteractAnimationComponent.h"
#include "InventoryEcosystem/01_Core/Component/BaseInventoryComponent.h"

#include "BaseInteractManager.generated.h"

/**
* Вызывается когда менеджер обновился
* Локально + Сервер
* @InteractManager - Менеджер взаимодействия
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractManagerUpdateSignature, class UBaseInteractManager*, InteractManager);

USTRUCT(BlueprintType)
struct FInteractManagerState
{
	GENERATED_BODY()
		
public:

	//Предыдущие состояние стейт машины
	ECharacterInteractState OldInteractState;

	/** Текущее состояние стейт машины */
	UPROPERTY()
	ECharacterInteractState CurrentInteractState;

	//Следующие состояние стейт машины
	ECharacterInteractState TargetInteractState;

	UPROPERTY()
	FGameplayTag CurrentInteractStateTag;

	void SetNewInteractComponent(UBaseInteractComponent* NewInteractComponent, UInteractAnimationComponent* InInteractAnimationComponent)
	{
		LastInteractComponent = CurrentInteractComponent;
		CurrentInteractComponent = NewInteractComponent;
		
		LastInteractAnimationComponent = CurrentInteractAnimationComponent;
		CurrentInteractAnimationComponent = InInteractAnimationComponent;
	}

	UBaseInteractComponent* GetCurrentInteractComponent() const
	{
		return CurrentInteractComponent;
	}

	UBaseInteractComponent* GetLastInteractComponent() const
	{
		return LastInteractComponent;
	}

	UInteractAnimationComponent* GetCurrentInteractAnimationComponent() const
	{
		return CurrentInteractAnimationComponent;
	}

	UInteractAnimationComponent* GetLastInteractAnimationComponent() const
	{
		return LastInteractAnimationComponent;
	}

	bool CanBeTrace() const
	{
		return CurrentInteractState == ECharacterInteractState::ECIS_None;
	}
protected:
	
	UPROPERTY()
	//Текущий объект взаимодействия
	UBaseInteractComponent* CurrentInteractComponent = nullptr;
	UPROPERTY()
	//Текущий компонент для проигрывания анимаций
	UInteractAnimationComponent* CurrentInteractAnimationComponent = nullptr;

	UPROPERTY()
	//Текущий объект взаимодействия
	UBaseInteractComponent* LastInteractComponent = nullptr;
	UPROPERTY()
	//Текущий компонент для проигрывания анимаций
	UInteractAnimationComponent* LastInteractAnimationComponent = nullptr;

};

static TAutoConsoleVariable<bool> CDrawDebugInteractedLine(TEXT("IES.Interact.ShowInteractTrace"), false, TEXT("Enable draw debug interact"), ECVF_Cheat);

UCLASS(Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent), HideCategories = ("Component", "Sockets", "Collision", "AssetUserData", "Component Tick", "ComponentTick", "Component Replication", "ComponentReplication", "Cooking", "Activation", "Variable"))
class INTERACTSYSTEM_API UBaseInteractManager : public UBaseInventoryComponent
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FInteractFocusSignature OnBeginFocus;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FInteractFocusSignature OnFocusLost;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FInteractedSignature OnInteracted;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FInteractedChangeStateSignature OnBeginInteract;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FInteractedChangeStateSignature OnEndInteract;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FInteractManagerUpdateSignature OnInteractManagerStateChange;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Настройки|Менеджер взаимодействие|Базовые",
		meta = (ClampMin = "1", ClampMax = "100", DisplayName = "Frequence", ForceUnits = "Hz"))
	float InteractionCheckFrequency = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Настройки|Менеджер взаимодействие|Базовые",	meta = (ClampMin = "100"))
	float MaxInteractDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Настройки|Менеджер взаимодействие|Базовые")
	FName InteractProfile = "Interacted";

	UPROPERTY(EditDefaultsOnly, Category = "Настройки|Менеджер взаимодействие|Базовые", meta = (ClampMin = "30"))
	float InteractionSphereSize = 30.f;
	
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Настройки|Менеджер взаимодействие|Управление", Meta = (DisplayThumbnail = false))
	// TObjectPtr<UInputAction> MoveAction;

	TArray<AActor*> AutoInteractArray;

	UPROPERTY(ReplicatedUsing = OnRep_InteractManagerState)
	FInteractManagerState InteractManagerState;
	
	//Цикличный таймер трейса, можно регулировать количество вызовов проверки взаимодействия в секунду
	UPROPERTY()
	FTimerHandle Timer_InteractTrace;

	//Таймер проверяющий сколько времени была зажата кнопка, а так же реализует механники автоматического взаимодействия по окончанию таймера
	UPROPERTY()
	FTimerHandle Timer_InteractPressed;

	//Цикличный таймер для движения, используется для направления персонажа (Аналог тика, только включается когда это нужно а не тикает постоянно)
	FTimerHandle Timer_MoveTo;

private:
	//Кешированая ссылка на камеру, для избежания лишних кастов
	UPROPERTY()
	UCameraComponent* PlayerCameraComponent;
	//Кешированая ссылка на камеру, для избежания лишних кастов
	UPROPERTY()
	EInteractType CachedTargetInteractType;
	
public:	
	UBaseInteractManager();

	/** Функция предназначена для вызова по нажатию кнопки взаимодействия*/
	UFUNCTION(BlueprintCallable, Category = "InteractSystem|InteractManager|Actions")
	void InteractPressed();

	/** Функция предназначена для вызова по нажатию кнопки взаимодействия*/
	UFUNCTION(BlueprintCallable, Category = "InteractSystem|InteractManager|Actions")
	void InteractReleased();

	UFUNCTION(BlueprintPure)
	UBaseInteractComponent* GetCurrentInteractComponent() {	return InteractManagerState.GetCurrentInteractComponent();	};

	UFUNCTION(BlueprintPure)
	UInteractAnimationComponent* GetCurrentInteractAnimationComponent() {	return InteractManagerState.GetCurrentInteractAnimationComponent();	};
	
	UFUNCTION(BlueprintPure)
	AActor* GetInteractedActor();

	//TODO: Сделать проверку через интерфейс
	UFUNCTION(BlueprintPure)
	bool IsCharacterInOtherAction() { return false; };

	/** Возвращает позицию удара трейса, по каналу Visible*/
	UFUNCTION(BlueprintPure)
	FVector DoCollisionTest(float AppendVector = 0.f);

	/** Возвращает позицию удара трейса, по каналу Visible*/
	UFUNCTION(BlueprintPure)
	bool DoCollisionTestHit(FHitResult& OutHitResult, float AppendVector = 0.f);

	/** */
	virtual void SetActive(bool bNewActive, bool bReset = false) override;
	
	/** */
	UFUNCTION(BlueprintCallable, Category = "UnrealInventorySystem|InteractManager|Tools")
	bool CharacterInActiveAction() { return (InteractManagerState.CurrentInteractState != ECharacterInteractState::ECIS_None); }

	/** */
	UFUNCTION(BlueprintCallable, Category = "InteractSystem|InteractManager|Tools")
	static UBaseInteractManager* GetInteractManager(AActor* FromActor);

	/** */
	UFUNCTION(BlueprintCallable, Category = "InteractSystem|InteractManager|Tools")
	ECharacterInteractState GetInteractState() { return InteractManagerState.CurrentInteractState; }

	/** */
	UFUNCTION(BlueprintCallable, Category = "InteractSystem|InteractManager|Actions")
	void CancelInteraction();

protected:
	virtual void BeginPlay() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/*
	* Вызывается при InteractPressed
	* Вызовится если есть с чем взаимодействовать */
	virtual bool BeginInteract();

	/*
	* Вызывается при InteractReleased
	* Вызовится если есть с чем взаимодействовать */
	virtual void EndInteract(UBaseInteractComponent* InteractComponent);

	/** Цикличный таймер, выполняет трейс взаимодействия игрока. */
	UFUNCTION()
	void PerformTrace();
	
	/** */
	void InteractByNotify();
	/** */
	bool IsNeedMoving();

	UFUNCTION()
	void OnRep_InteractManagerState();
	
	UFUNCTION()
	void OnAnimationCompleted(ACharacter* InCharacterInstigator, UInteractAnimationComponent* InteractAnimComponent, UAnimationInteractRule* InteractRules);

	UFUNCTION()
	void OnRep_LastAxis() {};
	
	/** */
	UFUNCTION()
	void MoveTo();
	
	/** */
	UFUNCTION()
	void MoveEnd();

	/** */
	bool SwitchInteractState(ECharacterInteractState NewState);

	virtual bool MoveToInteractPoint();

	/** */
	void AnimationInteractSuccessfull();

	/** */
	UFUNCTION()
	void NextInteractState();

	/** Вызывается при успешном взаимодействии */
	virtual void EventPreInteract(UBaseInteractComponent* InteractComponent, EInteractType InteractType);

	/** Вызывается при успешном взаимодействии */
	virtual void EventInteract(UBaseInteractComponent* InteractComponent, EInteractType InteractType);

	/** Вызывается при успешном взаимодействии */
	UFUNCTION()
	void PressedTimerInteract(EInteractType Interact);

	UFUNCTION(Server, Reliable)
	void Server_BeginInteract(UBaseInteractComponent* InteractComponent);
	void Server_BeginInteract_Implementation(UBaseInteractComponent* InteractComponent);

	UFUNCTION(Server, Reliable)
	void Server_EndInteract(UBaseInteractComponent* InteractComponent);
	void Server_EndInteract_Implementation(UBaseInteractComponent* InteractComponent);

	UFUNCTION(Server, Reliable)
	void Server_BeginFocus(UBaseInteractComponent* InteractComp);
	void Server_BeginFocus_Implementation(UBaseInteractComponent* InteractComp);

	UFUNCTION(Server, Reliable)
	void Server_EndFocus(UBaseInteractComponent* InteractComp);
	void Server_EndFocus_Implementation(UBaseInteractComponent* InteractComp);

	UFUNCTION(Server, Reliable)
	void Server_CancelInteraction();
	void Server_CancelInteraction_Implementation();

	UFUNCTION(Client, Reliable)
	void Client_CancelInteraction();
	void Client_CancelInteraction_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_RunAnimation(UInteractAnimationComponent* InteractComp);
	void NetMulticast_RunAnimation_Implementation(UInteractAnimationComponent* InteractComp);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_CancelAnimation(UInteractAnimationComponent* InteractComp);
	void NetMulticast_CancelAnimation_Implementation(UInteractAnimationComponent* InteractComp);

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
