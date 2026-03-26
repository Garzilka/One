#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/NetDriver.h"
#include "Engine/PendingNetGame.h"
#include "Net/UnrealNetwork.h"
#include "NativeGameplayTags.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Net/UnrealNetwork.h"
#include "Engine/NetDriver.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

 #include "InventoryEcosystem/01_Core/Library/InventoryCoreLogTools.h"

#include "BaseInventoryObject.generated.h"

#define LOG_BIO(type, text) UInventoryCoreLogTools::LOG(GetOwner(), this, type, text, __LINE__, __FUNCTION__)

UCLASS()
class INVENTORYCORE_API UBaseInventoryObject : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(Replicated)
	UObject* ParrentObject = nullptr;

	UPROPERTY(ReplicatedUsing = OnRep_Outer)
	UObject* RepOuter = nullptr;

	UPROPERTY()
	AActor* Owner = nullptr;

	UPROPERTY()
	ACharacter* CharacterOwner = nullptr;

	UPROPERTY()
	APlayerController* ControllerOwner = nullptr;

public:
	virtual bool Rename(const TCHAR* NewName = nullptr, UObject* NewOuter = nullptr, ERenameFlags Flags = REN_None) override;

	UFUNCTION(BlueprintPure)
	virtual AActor* GetOwner();

	UFUNCTION(BlueprintPure)
	APlayerController* GetOwningPlayer();

	UFUNCTION(BlueprintPure)
	ACharacter* GetOwningPlayerCharacter();

	UFUNCTION(BlueprintPure)
	bool HasAuthority() { return (GetOwner() && GetOwner()->HasAuthority()); }

	UFUNCTION(BlueprintPure)
	UObject* GetParrentObject() const { return ParrentObject; }

	UFUNCTION(BlueprintPure, Category = "UnrealInventory|Inventory|Item|Tools")
	virtual TArray<UObject*> GetAttachedSubObject() { return {}; }

	UFUNCTION(BlueprintCallable, Category = "UnrealInventory|Inventory|Item|Foundation")
	bool SwitchParrent(UObject* NewParrent);

	UFUNCTION(BlueprintCallable, Category = "UnrealInventory|Inventory|Item|Foundation")
	bool RemoveFromParrent();

	UFUNCTION(BlueprintCallable, Category = "UnrealInventory|Inventory|Item|Foundation")
	bool IsParrentEqual(UObject* NewParrent) { return (NewParrent == ParrentObject); }

	UFUNCTION(BlueprintCallable, Category = "UnrealInventory|Inventory|Item|Foundation")
	bool IsCanRemoveFromParrent();

	UFUNCTION(BlueprintCallable, Category = "UnrealInventory|Inventory|Item|Foundation")
	void ChangeOwner(AActor* InNewOuter);

	bool CanSerialize()
	{
		if ((!GEngine || !GetOwner() || HasAnyFlags(RF_ArchetypeObject)))
			return false;
		if ((GetOwner()->GetWorld() && GetOwner()->GetWorld()->WorldType == EWorldType::EditorPreview))
			return false;
		if (!HasAuthority())
			return false;
		return true;
	};
protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool IsSupportedForNetworking() const override { return true; };

	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, struct FOutParmRec* OutParms, FFrame* Stack) override;

	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;

	UFUNCTION()
	virtual void OnRep_Outer();
};
