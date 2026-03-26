#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryCoreLogTools.generated.h"

class IItemManagmentInterface;

UENUM()
enum class E_IES_LOG_TYPE : uint8
{
	EIEST_Display,
	EIEST_Warning,
	EIEST_Error
};

class UBaseInventoryObject;

UCLASS()
class INVENTORYCORE_API UInventoryCoreLogTools : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void LOG(AActor* Owner, UBaseInventoryObject* InObject, E_IES_LOG_TYPE TypeLog, FString Str, int Line, const char* Function);


	UFUNCTION()
	static void PrintString(FString Message, FColor Color = FColor::Blue);
	static FColor ErrorToColor(E_IES_LOG_TYPE ErrorType);
	static FString GetPlayerNickname(AActor* From);
	static FString GetNetMode(AActor* InActor);
	
};
