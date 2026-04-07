#pragma once

#include "CoreMinimal.h"
#include "InventoryEcosystem/02_InteractSystem/Objects/AnimationInteractRule.h"
#include "AnimatedInteraction.generated.h"

class UInteractAnimationComponent;

UCLASS()
class INTERACTSYSTEM_API UAnimatedInteraction : public UAnimationInteractRule
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Настройка анимаций", meta = (AllowPrivateAccess = "true", DisplayName = "Стартовая анимация персонажа"))
	UAnimMontage* TPSBeginAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Настройка анимаций", meta = (AllowPrivateAccess = "true", DisplayName = "Анимация взаимодействия", EditConditionHides))
	UAnimMontage* TPSInteractAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Настройка анимаций", meta = (AllowPrivateAccess = "true", DisplayName = "Анимация выхода"))
	UAnimMontage* TPSEndAnimMontage;
public:
	virtual void StartInteraction(UInteractAnimationComponent* Component, ACharacter* TargetPlayer) override;
	virtual void EndInteraction(UInteractAnimationComponent* Component, ACharacter* TargetPlayer) override;
	virtual void DoInteraction(UInteractAnimationComponent* Component, ACharacter* TargetPlayer) override;
	virtual UAnimMontage* GetPreviewAnimation() override {return TPSBeginAnimMontage;};

};
