// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryEcosystem/01_Core/Component/BaseInventorySceneComponent.h"
#include "InventoryEcosystem/02_InteractSystem/Data/InteractData.h"
#include "BaseInteractComponent.generated.h"



/**
* Вызывается когда объект попал в поле взаимодействия или вышел из него
* Локально + Сервер
* @Character - Кто взаимодйствует
* @InteractComponent - Компонент взаимодействия
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteractFocusSignature, class ACharacter*, CharacterInstigator, class UBaseInteractComponent*, InteractComponent);

/**
* Вызывается когда произошло взаимодействие
* Локально + Сервер
* @CharacterInstigator - Кто взаимодйствует
* @InteractComponent - Компонент взаимодействия
* @InteractType - Тип произошедшего взаимодействия
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FInteractedSignature, class ACharacter*, CharacterInstigator, class UBaseInteractComponent*, InteractComponent, EInteractType, InteractType);

/**
* Вызывается в начале и в конце взаимодействия (Нажатие и отжатие кнопки взаимодействия)
* Локально + Сервер
* @CharacterInstigator - Кто взаимодйствует
* @InteractComponent - Компонент взаимодействия
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteractedChangeStateSignature, class ACharacter*, CharacterInstigator, class UBaseInteractComponent*, InteractComponent);


UCLASS()
class INTERACTSYSTEM_API UBaseInteractComponent : public UBaseInventorySceneComponent
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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FInteractDescription Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBaseInteractSettings InteractSettings;

	UPROPERTY()
	float CurrentInteractTime = 0.f;

	UPROPERTY()
	bool InInteract = false;

public:
	UBaseInteractComponent() {};

	UFUNCTION(BlueprintPure)
	FBaseInteractSettings GetInteractSettings() const {return InteractSettings;};
	
	UFUNCTION(BlueprintPure, Category = "InventoryEcosystem|Interact|InteractComponent")
	FText GetInteractableNameText() { return Description.NameText; }

	UFUNCTION(BlueprintPure, Category = "InventoryEcosystem|Interact|InteractComponent")
	FText GetInteractableActionText() { return Description.ActionText; }

	UFUNCTION(BlueprintCallable, Category = "InventoryEcosystem|Interact|InteractComponent")
	void BeginFocus(ACharacter* CharacterInstigator);

	UFUNCTION(BlueprintCallable, Category = "InventoryEcosystem|Interact|InteractComponent")
	void EndFocus(ACharacter* CharacterInstigator);

	UFUNCTION(BlueprintCallable, Category = "InventoryEcosystem|Interact|InteractComponent")
	void Interact(ACharacter* CharacterInstigator, EInteractType InteractType);

	UFUNCTION(BlueprintCallable, Category = "InventoryEcosystem|Interact|InteractComponent")
	void StartInteract(ACharacter* CharacterInstigator);

	UFUNCTION(BlueprintCallable, Category = "InventoryEcosystem|Interact|InteractComponent")
	void EndInteract(ACharacter* CharacterInstigator);
	
	/**	*/
	UFUNCTION(BlueprintPure, Category = "InventoryEcosystem|Interact|InteractComponent")
	bool HasInteractFlag(EInteractType Type);

	/**	*/
	UFUNCTION(BlueprintPure, Category = "InventoryEcosystem|Interact|InteractComponent")
	bool CanInteractFor(ACharacter* CharacterInstigator);

	/**/
	UFUNCTION(BlueprintPure, Category = "InventoryEcosystem|Interact|InteractComponent")
	bool CanBePressed();

	UFUNCTION(BlueprintPure, Category = "InventoryEcosystem|Interact|InteractComponent")
	bool IsOutLine(UPrimitiveComponent* Primitive);
	
};
