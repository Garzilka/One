// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

/**
 * 
 */
UCLASS()
class INTERACTSYSTEM_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Init(class UBaseInteractComponent* Component);
	virtual void Init_Implementation(class UBaseInteractComponent* Component) { return; };

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Clear(class UBaseInteractComponent* Component);
	virtual void Clear_Implementation(class UBaseInteractComponent* Component) { return; };

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Refresh(class UBaseInteractComponent* Component);
	virtual void Refresh_Implementation(class UBaseInteractComponent* Component) { return; };
	
};
