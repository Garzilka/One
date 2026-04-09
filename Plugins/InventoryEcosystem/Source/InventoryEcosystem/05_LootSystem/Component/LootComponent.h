// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LootComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOOTSYSTEM_API ULootComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULootComponent();

protected:
	virtual void BeginPlay() override;
};
