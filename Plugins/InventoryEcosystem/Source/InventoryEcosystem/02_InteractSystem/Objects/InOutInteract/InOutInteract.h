#pragma once

#include "CoreMinimal.h"
#include "InventoryEcosystem/02_InteractSystem/Objects/AnimationInteractRule.h"
#include "InOutInteract.generated.h"

/**
 * 
 */
UCLASS()
class INTERACTSYSTEM_API UInOutInteract : public UAnimationInteractRule
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Настройка анимаций", meta = (AllowPrivateAccess = "true", DisplayName = "Стартовая анимация персонажа"))
	UAnimMontage* TPSBeginAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Настройка анимаций", meta = (AllowPrivateAccess = "true", DisplayName = "Анимация выхода"))
	UAnimMontage* TPSEndAnimMontage;

public:
	virtual void StartInteraction(class UInteractAnimationComponent* Component, ACharacter* TargetPlayer) override;
	virtual void EndInteraction(class UInteractAnimationComponent* Component, ACharacter* TargetPlayer) override;
	virtual UAnimMontage* GetPreviewAnimation() override {return TPSBeginAnimMontage;};
};