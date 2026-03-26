#pragma once


#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObjectManagmentInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UObjectManagmentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORYCORE_API IObjectManagmentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	bool I_RemoveObject(class UBaseInventoryObject* Item);

	UFUNCTION(BlueprintNativeEvent)
	bool I_CanRemoveObject(class UBaseInventoryObject* Item);
};