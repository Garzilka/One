// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InventoryEcosystem/02_InteractSystem/Component/BaseInteractManager.h"

#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystemInterface.h"
#include "GameplayCharacter.generated.h"

UCLASS(BlueprintType, HideCategories = ("Actor Tick", "Rendering", "Replication", "Input", "Actor", "Base", "Collision", "Shape", "MeshComponent", "Cooking", "LOD", "WorldPartition"))
class ONE_API AGameplayCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components)
	UBaseInteractManager* InteractManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Настройки|Персонаж|Управление")
	UInputMappingContext* InputMapping;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Настройки|Персонаж|Управление", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Настройки|Персонаж|Управление", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> LookMouseAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Настройки|Персонаж|Управление", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> InteractAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Настройки|Персонаж|Управление", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> Action_AIM;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Настройки|Персонаж|Управление|Слоты использования", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> UseSlot_01;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Настройки|Персонаж|Управление|Слоты использования", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> UseSlot_02;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Настройки|Персонаж|Управление", Meta = (DisplayThumbnail = false))
	FVector2D MouseSensitivity{-1.f, 1.f};
	
protected:
	virtual void BeginPlay() override;

public:	
	AGameplayCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Input_OnMove(const FInputActionValue& ActionValue);
	virtual void Input_LookMouse(const FInputActionValue& ActionValue);
	virtual void Input_Interact(const FInputActionValue& ActionValue);
	virtual void Input_AIM(const FInputActionValue& ActionValue);
	
	virtual void Input_UseSlot_01(const FInputActionValue& ActionValue);
	virtual void Input_UseSlot_02(const FInputActionValue& ActionValue);

};
