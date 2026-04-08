// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SkeletalMeshComponent.h"
#include "InventoryEcosystem/02_InteractSystem/Data/InteractData.h"
#include "InventoryEcosystem/02_InteractSystem/Objects/AnimationInteractRule.h"
#include "InteractAnimationComponent.generated.h"

/**
* Вызывается когда объект попал в поле взаимодействия или вышел из него (Локально + Сервер)
* @Character - Кто взаимодйствует
* @InteractComponent - Референс на себя
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAnimationInteractComponentSignature, class ACharacter*, InCharacterInstigator, class UInteractAnimationComponent*, InteractAnimComponent, class UAnimationInteractRule*, InteractRules);

/**
* Вызывается когда объект попал в поле взаимодействия или вышел из него (Локально + Сервер)
* @Character - Кто взаимодйствует
* @InteractComponent - Референс на себя
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractAnimationComponentChangeSignature, class UInteractAnimationComponent*, InteractAnimComponent);


UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent),
	HideCategories = ("SkeletalMesh", "Lighting", "Clothing", "Physics", "Mesh", "Animation", "Component", "Sockets", "Collision", "AssetUserData", "Component Tick", "LeaderPoseComponent",
		 "ComponentTick", "Component Replication", "ComponentReplication", "Cooking", "Activation",  "SkinWeight",  "Rendering", "AnimationRig", "Deformer", "HLOD", "Variable",
		 "VirtualTexture", "SkinWeights", "Navigation", "Optimization", "RayTracing", "Materials", "MaterialParameters", "TextureStreaming", "Mobile", "LevelOfDetail"))
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
	FInteractAnimationComponentChangeSignature OnChangeCurrentInteractor;
	
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Настройка анимаций", meta = (DisplayName = "Тип позиции взаимодействия",
		EditCondition = "IsClassInstalled()"))
	EInteractPositionType PositionType = EInteractPositionType::EIPT_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Настройка анимаций",
		meta = (DisplayName = "Радиус взаимодействия", EditCondition = "IsClassInstalled() && PositionType == EInteractPositionType::EIPT_Radius", EditConditionHides, ToolTip = "Перед проигрыванием анимации, персонаж должен добраться до точки"))
	float InteractRadius = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Настройка анимаций", meta = (DisplayName = "Тэг взаимодействия", ToolTip = "Если тэг пустой, то персонаж автоматически выйдет из состояния взаимодействия",
		EditCondition = "IsClassInstalled()"))
	FGameplayTag InteractState = FGameplayTag();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Настройка анимаций", meta = (DisplayName = "Настройки анимаций"))
	UAnimationInteractRule* AnimationRule;
	
	UPROPERTY(ReplicatedUsing = OnRep_UpdateInteractor)
	ACharacter* CurrentInteractor = nullptr;
	
public:
	UInteractAnimationComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void StartInteraction(ACharacter* TargetPlayer);

	UFUNCTION(BlueprintCallable)
	virtual void EndInteraction(ACharacter* TargetPlayer);
	
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
	
	UFUNCTION(BlueprintPure, Category = "InventoryEcosystem|Interact|InteractComponent")
	bool IsOccupied() const { return IsValid(CurrentInteractor); };
	
	UFUNCTION(BlueprintPure, Category = "InventoryEcosystem|Interact|InteractComponent")
	ACharacter* GetCurrentInteractor() const { return CurrentInteractor; };

protected:
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	bool IsClassInstalled();

	UFUNCTION()
	void OnRep_UpdateInteractor();

	UFUNCTION()
	void Event_Interacted(ACharacter* CharacterInstigator, class UAnimationInteractRule* InteractRules);
	UFUNCTION()
	void Event_StartAnimation(ACharacter* CharacterInstigator, class UAnimationInteractRule* InteractRules);
	UFUNCTION()
	void Event_EndAnimation(ACharacter* CharacterInstigator, class UAnimationInteractRule* InteractRules);
	UFUNCTION()
	void Event_CancelInteract(ACharacter* CharacterInstigator, class UAnimationInteractRule* InteractRules);
};
