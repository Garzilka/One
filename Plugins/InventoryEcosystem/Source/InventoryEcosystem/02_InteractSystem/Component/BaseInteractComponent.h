// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryEcosystem/01_Core/Component/BaseInventorySceneComponent.h"
#include "InventoryEcosystem/02_InteractSystem/Data/InteractData.h"
#include "BaseInteractComponent.generated.h"




UCLASS(Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
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
	FInteractSettings InteractSettings;

	UPROPERTY()
	float CurrentInteractTime = 0.f;

	UPROPERTY()
	bool InInteract = false;

public:
	UBaseInteractComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
	FInteractSettings GetInteractSettings() const {return InteractSettings;};
	
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
	bool HasInteractFlag(EInteractType Type) const;

	/**	*/
	UFUNCTION(BlueprintPure, Category = "InventoryEcosystem|Interact|InteractComponent")
	bool CanInteractFor(ACharacter* CharacterInstigator) const;

	/**/
	UFUNCTION(BlueprintPure, Category = "InventoryEcosystem|Interact|InteractComponent")
	bool CanBePressed() const;

	UFUNCTION(BlueprintPure, Category = "InventoryEcosystem|Interact|InteractComponent")
	bool IsOutLine(UPrimitiveComponent* Primitive);
	
	UFUNCTION(BlueprintPure, Category = "InventoryEcosystem|Interact|InteractComponent")
	float GetTimeToInteract() const;
	
	UFUNCTION(BlueprintPure, Category = "InventoryEcosystem|Interact|InteractComponent")
	float GetInteractPercent() const;
	
};
