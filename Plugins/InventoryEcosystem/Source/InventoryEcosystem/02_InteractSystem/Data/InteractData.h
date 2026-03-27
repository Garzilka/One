#pragma once

#include "CoreMinimal.h"
#include "InteractData.generated.h"


UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EInteractType : uint8
{
	EIT_None			= 0			UMETA(Hidden),
	EIT_Press			= 1	<< 0	UMETA(DisplayName = "Нажатие"),
	EIT_Release			= 1 << 1	UMETA(DisplayName = "Отжатие"),
	EIT_Pressed			= 1	<< 2	UMETA(DisplayName = "Зажать на время"),
	EIT_CirculPressed	= 1	<< 3	UMETA(DisplayName = "Цикличное"),
	EIT_Released		= 1	<< 4	UMETA(DisplayName = "Отжать после времени"),
	EIT_QTE				= 1	<< 5	UMETA(DisplayName = "QTE"),
};
ENUM_RANGE_BY_FIRST_AND_LAST(EInteractType, EInteractType::EIT_Press, EInteractType::EIT_QTE);

UENUM(BlueprintType)
enum class EInteractTypeTimeDegradation : uint8
{
	EITTD_None			UMETA(DisplayName = "None"),
	EITTD_Clear			UMETA(DisplayName = "Сбросить"),
	EITTD_Reverse		UMETA(DisplayName = "Откат"),
};


/**
* Вызывается когда объект попал в поле взаимодействия или вышел из него
* Локально + Сервер
* @Character - Кто взаимодйствует
* @InteractComponent - Компонент взаимодействия
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FInteractFocusSignature, class ACharacter*, CharacterInstigator, class UBaseInteractComponent*, InteractComponent, class AActor*, Actor);

/**
* Вызывается когда произошло взаимодействие
* Локально + Сервер
* @CharacterInstigator - Кто взаимодйствует
* @InteractComponent - Компонент взаимодействия
* @InteractType - Тип произошедшего взаимодействия
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FInteractedSignature, class ACharacter*, CharacterInstigator, class UBaseInteractComponent*, InteractComponent, EInteractType, InteractType);

/**
* Вызывается в начале и в конце взаимодействия (Нажатие и отжатие кнопки взаимодействия)
* Локально + Сервер
* @CharacterInstigator - Кто взаимодйствует
* @InteractComponent - Компонент взаимодействия
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteractedChangeStateSignature, class ACharacter*, CharacterInstigator, class UBaseInteractComponent*, InteractComponent);




USTRUCT(BlueprintType)
struct FInteractDescription
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText NameText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ActionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UInteractWidget> InteractWidget;
};

USTRUCT(BlueprintType)
struct FInteractSettings
{
	GENERATED_BODY()


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "/Script/InteractSystem.EInteractType"))
	int32 ActionMenu = (uint8)EInteractType::EIT_Release;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractionTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteractTypeTimeDegradation TypeTimeDegradation = EInteractTypeTimeDegradation::EITTD_Clear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpeedOfRevers = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0.f))
	float InteractionDistance = 700.f;
};