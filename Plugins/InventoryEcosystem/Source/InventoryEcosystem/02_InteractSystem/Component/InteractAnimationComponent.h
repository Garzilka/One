// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SkeletalMeshComponent.h"
#include "InventoryEcosystem/02_InteractSystem/Data/InteractData.h"
#include "InteractAnimationComponent.generated.h"

/**
* Вызывается когда объект попал в поле взаимодействия или вышел из него (Локально + Сервер)
* @Character - Кто взаимодйствует
* @InteractComponent - Референс на себя
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAnimationInteractComponentSignature, class ACharacter*, CharacterInstigator, class UInteractAnimationComponent*, InteractAnimComponent, class UAnimationInteractRule*, InteractRules);


UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent),
	HideCategories = ("SkeletalMesh", "Lighting", "Clothing", "Physics", "Mesh", "Animation", "Component", "Sockets", "Collision", "AssetUserData", "Component Tick", "LeaderPoseComponent",
		 "ComponentTick", "Component Replication", "ComponentReplication", "Cooking", "Activation",  "SkinWeight",  "Rendering", "AnimationRig", "Deformer", "HLOD", "Variable",
		 "VirtualTexture", "SkinWeights", "Navigation", "Optimization", "RayTracing", "MaterialParameters", "TextureStreaming", "Mobile", "LevelOfDetail"))
class INTERACTSYSTEM_API UInteractAnimationComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FAnimationInteractComponentSignature OnInteracted;
	
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FAnimationInteractComponentSignature OnStartAnimation;
	
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FAnimationInteractComponentSignature OnEndAnimation;
	
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FAnimationInteractComponentSignature OnInteractAnimation;
	
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Настройка анимаций",
		meta = (DisplayPriority = -1, AllowPrivateAccess = "true", DisplayName = "Тип позиции взаимодействия", ToolTip = "Перед проигрыванием анимации, персонаж должен добраться до точки"))
	EInteractPositionType PositionType = EInteractPositionType::EIPT_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Настройка анимаций",
		meta = (DisplayPriority = -1, AllowPrivateAccess = "true", DisplayName = "Радиус взаимодействия", EditCondition = "PositionType == EInteractPositionType::EIPT_Radius", EditConditionHides, ToolTip = "Перед проигрыванием анимации, персонаж должен добраться до точки"))
	float InteractRadius = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Настройка анимаций", meta = (DisplayPriority = -1, AllowPrivateAccess = "true", DisplayName = "Тэг взаимодействия"))
	FGameplayTag InteractState = FGameplayTag();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Настройка анимаций", meta = (DisplayName = "Настройки анимаций"))
	UAnimationInteractRule* AnimationRule;
	
public:
	UInteractAnimationComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void RunInteractAnimation() const {};
	
	UFUNCTION(BlueprintPure, Category = "UnrealInventory|Interact|BaseInteractComponent")
	FTransform GetInteractTransform(bool IsRelative = false);

	UFUNCTION(BlueprintPure)
	EInteractPositionType GetPositionType() const { return PositionType; }

	UFUNCTION(BlueprintPure)
	float GetInteractRadius() const { return InteractRadius; }

	UFUNCTION(BlueprintPure)
	FGameplayTag GetInteractState() const { return InteractState; }
	
	UFUNCTION(BlueprintPure, Category = "InventoryEcosystem|Interact|InteractComponent")
	UAnimationInteractRule* GetAnimationInteractRule() const { return AnimationRule; };

protected:
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION()
	void Event_Interacted(ACharacter* CharacterInstigator, class UAnimationInteractRule* InteractRules);
	UFUNCTION()
	void Event_StartAnimation(ACharacter* CharacterInstigator, class UAnimationInteractRule* InteractRules);
	UFUNCTION()
	void Event_EndAnimation(ACharacter* CharacterInstigator, class UAnimationInteractRule* InteractRules);
	UFUNCTION()
	void Event_InteractAnimation(ACharacter* CharacterInstigator, class UAnimationInteractRule* InteractRules);
};
