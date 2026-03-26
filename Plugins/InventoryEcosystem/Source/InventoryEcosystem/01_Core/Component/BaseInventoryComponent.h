// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "BaseInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYCORE_API UBaseInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBaseInventoryComponent();

	UFUNCTION(BlueprintPure)
	bool HasAuthority() const { return (GetOwner() && GetOwner()->HasAuthority()); };

	UFUNCTION(BlueprintPure)
	bool IsLocallyControlled();

	UFUNCTION(BlueprintPure)
	bool IsLocallySpectator();

	UFUNCTION(BlueprintCallable)
	ACharacter* GetOwningPlayerCharacter();

	UFUNCTION(BlueprintCallable)
	APlayerController* GetOwningPlayerController();

	void ChangeObjectOuter(UObject* ItemRef, AActor* InNewOuter);
	
protected:
	bool CanSerialize() const
	{
		if ((!GEngine || !GetOwner() || HasAnyFlags(RF_ArchetypeObject)))
			return false;
		if ((GetWorld() && GetWorld()->WorldType == EWorldType::EditorPreview))
			return false;
		if (!HasAuthority())
			return false;
		return true;
	};
	
private:
	UPROPERTY()
	ACharacter* CharacterOwner = nullptr;

	UPROPERTY()
	APlayerController* ControllerOwner = nullptr;

};
