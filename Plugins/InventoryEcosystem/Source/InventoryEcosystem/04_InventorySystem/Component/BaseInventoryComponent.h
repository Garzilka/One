// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYSYSTEM_API UBaseInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBaseInventoryComponent();

protected:
	virtual void BeginPlay() override;
		
};
