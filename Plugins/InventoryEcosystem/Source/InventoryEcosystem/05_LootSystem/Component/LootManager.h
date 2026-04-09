// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LootManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOOTSYSTEM_API ULootManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULootManager();

protected:
	virtual void BeginPlay() override;
};
