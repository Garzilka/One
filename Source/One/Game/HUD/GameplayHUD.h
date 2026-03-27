// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/HUD.h"
#include "GameFramework/Character.h"
#include "GameplayHUD.generated.h"


UENUM(BlueprintType)
enum class EUIState : uint8
{
	EUI_Game						UMETA(DisplayName = "Game"),
	EUI_GameMenu				    UMETA(DisplayName = "GameMenu"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeUIStateSignature, EUIState, WidgetMenuType, EUIState, OldWidgetMenuType);

UCLASS()
class ONE_API AGameplayHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FChangeUIStateSignature OnChangeUIState;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Настройки|Управление", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> Action_Menu;
	
	EUIState CurrentStateType = EUIState::EUI_Game;
	EUIState OldStateType = EUIState::EUI_Game;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> GameWidgetClass;
	
	UPROPERTY()
	ACharacter* PlayerCharacter = nullptr;

	UPROPERTY()
	UUserWidget* GameWidget;
public:
	UFUNCTION(BlueprintPure)
	EUIState GetUIState() const { return CurrentStateType; };

	UFUNCTION(BlueprintCallable)
	void SetUIState(EUIState NewState);

	UFUNCTION(BlueprintPure)
	bool IsActiveState(EUIState InState) const { return (CurrentStateType == InState); };

	
protected:
	AGameplayHUD();

	virtual void BeginPlay() override;

	void OpenMenu();

	void SetGame();
	void SetMenu();

	ACharacter* GetPlayerCharacter();
};
