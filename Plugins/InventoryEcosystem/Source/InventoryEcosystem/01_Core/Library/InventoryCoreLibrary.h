#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"

#include "InventoryCoreLibrary.generated.h"

class IObjectManagmentInterface;

UCLASS()
class INVENTORYCORE_API UInventoryCoreLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static IObjectManagmentInterface* HasManagmentInterface(UObject* TargetObject);

	UFUNCTION(BlueprintPure, Category = "InventoryCoreLibrary")
	static APlayerState* GetPlayerState(AActor* Owner);
	static APlayerState* GetPlayerState(ACharacter* Owner);
	static APlayerState* GetPlayerState(APlayerController* Owner);
	
	UFUNCTION(BlueprintPure, Category = "InventoryCoreLibrary")
	static FString GetPlayerNickname(AActor* From);
	
	static bool IsInGameView();
	
};