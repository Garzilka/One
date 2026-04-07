#pragma once
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UObject/NoExportTypes.h"
#include "InteractSystemSettings.generated.h"


UCLASS(Config = "Game", defaultconfig, meta = (DisplayName = "InventorySettings"))
class INTERACTSYSTEM_API UInteractSystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(Config, EditAnywhere, Category = "InteractSettings")
	TSoftObjectPtr<USkeletalMesh> PreviewInteractMesh;


	static const UInteractSystemSettings* Get();
};