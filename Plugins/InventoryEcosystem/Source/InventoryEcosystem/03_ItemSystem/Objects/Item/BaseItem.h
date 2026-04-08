#pragma once
#include "InventoryEcosystem/01_Core/Object/BaseInventoryObject.h"
#include "BaseItem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemUpdateSignature, class UBaseItem*, Item);

UCLASS(Abstract, BlueprintType, Blueprintable)
class ITEMSYSTEM_API UBaseItem : public UBaseInventoryObject
{
	GENERATED_BODY()
	
public:

};
