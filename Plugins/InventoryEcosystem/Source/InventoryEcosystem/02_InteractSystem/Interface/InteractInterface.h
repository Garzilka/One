#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"


UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

class INTERACTSYSTEM_API IInteractInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	bool I_CanViewWidget(ACharacter* Instigator, class UBaseInteractComponent* Component);
	bool I_CanViewWidget_Implementation(ACharacter* Instigator, class UBaseInteractComponent* Component) { return true; }

	UFUNCTION(BlueprintNativeEvent)
	bool I_CanInteract(ACharacter* Instigator, class UBaseInteractComponent* Component);
	bool I_CanInteract_Implementation(ACharacter* Instigator, class UBaseInteractComponent* Component) { return true; }

	UFUNCTION(BlueprintNativeEvent)
	bool I_PlayAnimationBeginningInteraction(ACharacter* Instigator, class UBaseInteractComponent* Component);

	UFUNCTION(BlueprintNativeEvent)
	bool I_PlayIdleInteractAnimation(ACharacter* Instigator, class UBaseInteractComponent* Component);

	UFUNCTION(BlueprintNativeEvent)
	bool I_PlayOutAnimationInteraction(ACharacter* Instigator, class UBaseInteractComponent* Component);

	UFUNCTION(BlueprintNativeEvent)
	bool I_PlayInteractAnimation(ACharacter* Instigator, class UBaseInteractComponent* Component);

};
