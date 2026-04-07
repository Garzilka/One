#pragma once
#include "InventoryEcosystem/01_Core/Object/BaseInventoryObject.h"
#include "InventoryEcosystem/02_InteractSystem/Data/InteractData.h"
#include "AnimationInteractRule.generated.h"


class UInteractAnimationComponent;

/**
* Вызывается когда объект попал в поле взаимодействия или вышел из него (Локально + Сервер)
* @Character - Кто взаимодйствует
* @InteractComponent - Референс на себя
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAnimationInteractRuleSignature, class ACharacter*, CharacterInstigator, class UAnimationInteractRule*, InteractRules);

UCLASS(EditInlineNew, DefaultToInstanced)
class INTERACTSYSTEM_API UAnimationInteractRule : public UBaseInventoryObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FAnimationInteractRuleSignature OnInteracted;
	
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FAnimationInteractRuleSignature OnStartAnimation;
	
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FAnimationInteractRuleSignature OnEndAnimation;
	
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FAnimationInteractRuleSignature OnInteractAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Настройка анимаций")
	float PreviewTime = 0.f;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Настройка анимаций", meta = (DisplayPriority = -1, AllowPrivateAccess = "true", DisplayName = "Когда вызвать взаимодействие"))
	EInteractCallType InteractCallType = EInteractCallType::EICT_BeforeInteractAnimation;
	
	UPROPERTY()
	TMap<ACharacter*, FTimerHandle> CurrentInteractions;

public:

	UFUNCTION(BlueprintCallable)
	virtual void StartInteraction(UInteractAnimationComponent* Component, ACharacter* TargetPlayer) { return; }

	UFUNCTION(BlueprintCallable)
	virtual void EndInteraction(UInteractAnimationComponent* Component, ACharacter* TargetPlayer) { return; }

	UFUNCTION(BlueprintCallable)
	virtual void DoInteraction(UInteractAnimationComponent* Component, ACharacter* TargetPlayer) { return; }

	UFUNCTION(BlueprintPure)
	EInteractCallType GetInteractCallType() const { return InteractCallType; }

	virtual UAnimMontage* GetPreviewAnimation() {return nullptr;};
protected:
	UFUNCTION()
	void InteractCallback(ACharacter* Target);

	void RunInteract(float Time, UInteractAnimationComponent* Component, ACharacter* TargetPlayer);
	
};
