#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractManagerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INTERACTSYSTEM_API IInteractManagerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	bool I_IsCharacterInOtherAction();

};
