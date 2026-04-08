#pragma once
#include "InventoryEcosystem/01_Core/Object/CoreInventoryObject.h"
#include "InventoryEcosystem/02_InteractSystem/Data/InteractData.h"
#include "AnimationInteractRule.generated.h"


class UInteractAnimationComponent;

/**
* Вызывается когда объект попал в поле взаимодействия или вышел из него (Локально + Сервер)
* @Character - Кто взаимодйствует
* @InteractComponent - Референс на себя
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAnimationInteractRuleSignature, class ACharacter*, CharacterInstigator, class UAnimationInteractRule*, InteractRules);

UCLASS(EditInlineNew, DefaultToInstanced, Abstract)
class INTERACTSYSTEM_API UAnimationInteractRule : public UCoreInventoryObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FAnimationInteractRuleSignature OnInteracted;
	
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FAnimationInteractRuleSignature OnCancelInteract;
	
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FAnimationInteractRuleSignature OnStartAnimation;
	
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FAnimationInteractRuleSignature OnEndAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Настройка анимаций")
	float PreviewTime = 0.f;

protected:
	
	UPROPERTY()
	FTimerHandle Timer_TimeToInteract;
	
	UPROPERTY()
	FTimerHandle Timer_CancelInteract;
public:

	UFUNCTION()
	virtual void StartInteraction(ACharacter* TargetPlayer) { return; }

	UFUNCTION()
	virtual void EndInteraction(ACharacter* TargetPlayer) { return; }

	virtual UAnimMontage* GetPreviewAnimation() {return nullptr;};
protected:
	UFUNCTION()
	void InteractCallback(ACharacter* Target);

	void RunInteract(UAnimMontage* Montage, ACharacter* TargetPlayer);
	
	UFUNCTION()
	void CancelInteractCallback(ACharacter* Target);

	void CancelInteract(UAnimMontage* Montage, ACharacter* TargetPlayer);
	
};
