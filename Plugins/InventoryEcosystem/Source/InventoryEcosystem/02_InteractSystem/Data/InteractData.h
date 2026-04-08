#pragma once

#include "CoreMinimal.h"
#include "InteractData.generated.h"

USTRUCT(BlueprintType)
struct FBlockInteractInfo
{
	GENERATED_BODY()

public:
	FBlockInteractInfo() { Target = nullptr; CanViewInteractWidget = false; };
	FBlockInteractInfo(ACharacter* InTarget) { Target = InTarget; CanViewInteractWidget = false; };
	FBlockInteractInfo(ACharacter* InTarget, bool InCanView) { Target = InTarget; CanViewInteractWidget = InCanView; };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ACharacter* Target = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool CanViewInteractWidget = false;

	friend bool operator==(const FBlockInteractInfo& Left, const ACharacter* Right)
	{
		return (Left.Target == Right);
	}

	friend bool operator==(const FBlockInteractInfo& Left, const FBlockInteractInfo& Right)
	{
		return (Left.Target == Right.Target);
	}
};


UENUM(BlueprintType)
enum class EInteractPositionType : uint8
{
	EIPT_None					UMETA(DisplayName = "Нет"),
	EIPT_Point					UMETA(DisplayName = "Точка взаимодействия"),
	EIPT_Radius					UMETA(DisplayName = "Радиус взаимодействия"),
};

UENUM(BlueprintType)
enum class ECharacterInteractState : uint8
{
	ECIS_None					UMETA(DisplayName = "None"),
	ECIS_MovingToInteractPoint	UMETA(DisplayName = "Двигается до точки взаимодействия"),
	ECIS_InteractState			UMETA(DisplayName = "Взаимодействует"),
	ECIS_AwaitPlayerInput		UMETA(DisplayName = "Ожидает действий игрока"),
};

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

	UPROPERTY(EditDefaultsOnly,	meta = (DisplayName = "Приоритет", ToolTip = "Чем больше цифра, тем больше приоритет"))
	int32 Priority = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "/Script/InteractSystem.EInteractType", DisplayName = "Тип взаимодействия"))
	int32 ActionMenu = (uint8)EInteractType::EIT_Release;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Время взаимодействия"))
	float InteractionTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Тип отката таймера"))
	EInteractTypeTimeDegradation TypeTimeDegradation = EInteractTypeTimeDegradation::EITTD_Clear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Множитель реверса"))
	float SpeedOfRevers = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 50.f, UIMin = 50.f, DisplayName = "Дистанция взаимодействия"))
	float InteractionDistance = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  
		meta = (DisplayName = "Взаимодействует только один", Tooltip = "Если взаимодействие кто-то производит, взаимодействовать с объектом нельзя"))
	bool bIsOnlyOneInteractor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsOnlyOneInteractor", EditConditionHides,
			DisplayName = "Другой может видеть виджет", ToolTip = "Если игрок выполняет действие, другой игрок может видеть прогресс"))
	bool bIsAnotherCanSeeWidget = true;
};

USTRUCT(BlueprintType)
struct FInteractAngleSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Проверка по углу"))
	bool bWithAngleCheck = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Debug"))
	bool bEnableDebug = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bWithAngleCheck", EditConditionHides,
		DisplayName = "Допустимый угол взаимодействия", ClampMin = 5.f, ClampMax = 360.f, UIMin = 5.f, UIMax = 360.f))
	float InteractAngle = 360.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,  meta = (EditCondition = "bWithAngleCheck", EditConditionHides,
		DisplayName = "Угол смещения", ClampMin = -180.f, ClampMax = 180.f, UIMin = -180.f, UIMax = 180.f))
	float InteractAngleOffset = 0.f;
};