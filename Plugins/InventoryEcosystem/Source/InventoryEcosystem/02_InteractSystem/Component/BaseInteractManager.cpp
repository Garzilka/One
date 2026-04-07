// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInteractManager.h"

#include "InteractAnimationComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InventoryEcosystem/02_InteractSystem/Library/InteractSystemLibrary.h"

#pragma region ENGINE 

UBaseInteractManager::UBaseInteractManager()
{
	SetIsReplicatedByDefault(true);
	SetIsReplicated(true);
	PrimaryComponentTick.bCanEverTick = true;
}

void UBaseInteractManager::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocallyControlled()) return;
	
	GetWorld()->GetTimerManager().SetTimer(Timer_InteractTrace, this, &UBaseInteractManager::PerformTrace, (1.f / InteractionCheckFrequency), true);	
}

void UBaseInteractManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, "Velocity: X: " + FString::FromInt(GetPlayerDirection().X) + " Y: " + FString::FromInt(GetPlayerDirection().Y));
	// GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, "Collision Type \"Static\": Can block interact! Set CollisionType \"Dynamic\" for Volume");
}

void UBaseInteractManager::SetActive(bool bNewActive, bool bReset)
{
	if (bNewActive)
	{
		if (GetWorld() && !GetWorld()->GetTimerManager().IsTimerActive(Timer_InteractTrace))
			GetWorld()->GetTimerManager().SetTimer(Timer_InteractTrace, this, &UBaseInteractManager::PerformTrace, (1.f / InteractionCheckFrequency), true);
	}
	else
	{
		if (GetWorld() && !GetWorld()->GetTimerManager().IsTimerActive(Timer_InteractTrace))
			GetWorld()->GetTimerManager().ClearTimer(Timer_InteractTrace);
		InteractionNone();
	}
	Super::SetActive(bNewActive, bReset);
}

void UBaseInteractManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UBaseInteractManager, InteractManagerState);
}

#pragma endregion ENGINE

#pragma region INPUT

void UBaseInteractManager::InteractPressed()
{
	if (!GetCurrentInteractComponent() || !GetOwningPlayerCharacter()) return;

	if (!GetCurrentInteractComponent()->CanInteractFor(GetOwningPlayerCharacter())) return;

	if (!HasAuthority())
	{
		Server_BeginInteract(GetCurrentInteractComponent());
	}

	BeginInteract();
}

void UBaseInteractManager::Server_BeginInteract_Implementation(UBaseInteractComponent* InteractComponent)
{
	if (!IsValid(InteractComponent)) return;
	InteractManagerState.SetNewInteractComponent(InteractComponent, GetInteractAnimationComponent(InteractComponent));
	BeginInteract();
}

bool UBaseInteractManager::BeginInteract()
{
	if (!GetWorld()) return false;
	if (!GetCurrentInteractComponent()) return false;
    
	GetCurrentInteractComponent()->StartInteract(GetOwningPlayerCharacter());
    
	if (GetCurrentInteractComponent() && GetCurrentInteractComponent()->HasInteractFlag(EInteractType::EIT_Press))
	{
		EventPreInteract(GetCurrentInteractComponent(), EInteractType::EIT_Press);
		return true;
	}
    
	if (GetCurrentInteractComponent() && (GetCurrentInteractComponent()->HasInteractFlag(EInteractType::EIT_Pressed) || GetCurrentInteractComponent()->HasInteractFlag(EInteractType::EIT_CirculPressed)))
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, TEXT("PressedTimerInteract"), EInteractType::EIT_Pressed);			
		GetWorld()->GetTimerManager().SetTimer(Timer_InteractPressed, Delegate, GetCurrentInteractComponent()->GetTimeToInteract(), false);
		return true;
	}
	return false;
}


void UBaseInteractManager::InteractReleased()
{
	if (!GetCurrentInteractComponent() || !GetOwningPlayerCharacter()) return;

	if (IsCharacterInOtherAction()) return;

	if (!GetCurrentInteractComponent()->CanInteractFor(GetOwningPlayerCharacter())) return;

	if (!HasAuthority())
	{
		Server_EndInteract(GetCurrentInteractComponent());
	}

	EndInteract(GetCurrentInteractComponent());
}

void UBaseInteractManager::Server_EndInteract_Implementation(UBaseInteractComponent* InteractComponent)
{
	if (!GetCurrentInteractComponent()) return;
	EndInteract(InteractComponent);
}

void UBaseInteractManager::EndInteract(UBaseInteractComponent* InteractComponent)
{
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(Timer_InteractPressed))
	{
		GetWorld()->GetTimerManager().ClearTimer(Timer_InteractPressed);
	}
    
	if (InteractComponent && InteractComponent->HasInteractFlag(EInteractType::EIT_Release) && (InteractComponent->GetTimeToInteract() - 0.1f) >= 0.f)
	{
		EventPreInteract(InteractComponent, EInteractType::EIT_Release);
	}
	if (InteractComponent && InteractComponent->HasInteractFlag(EInteractType::EIT_Released))
	{
		if ((InteractComponent->GetTimeToInteract() - 0.1f) <= 0.f)
		{
			EventPreInteract(InteractComponent, EInteractType::EIT_Released);
		}
	}
	if (InteractComponent)
		InteractComponent->EndInteract(GetOwningPlayerCharacter());
}

void UBaseInteractManager::EventPreInteract(UBaseInteractComponent* InteractComponent, EInteractType InteractType)
{
	if (!IsValid(InteractComponent)) return;
	
	CachedTargetInteractType = InteractType;
	if (!GetCurrentInteractAnimationComponent())
	{
		EventInteract(InteractComponent, InteractType);
		return;
	}
	
	if (InteractManagerState.CurrentInteractState != ECharacterInteractState::ECIS_InteractState)
	{
		SwitchInteractState(ECharacterInteractState::ECIS_MovingToInteractPoint);
		return;
	}
	
	GetCurrentInteractAnimationComponent()->RunInteractAnimation();
}


void UBaseInteractManager::EventInteract(UBaseInteractComponent* InteractComponent, EInteractType InteractType)
{
	if (!InteractComponent) return;
	OnInteracted.Broadcast(GetOwningPlayerCharacter(), InteractComponent, InteractType);
	InteractComponent->Interact(GetOwningPlayerCharacter(), InteractType);
}

void UBaseInteractManager::InteractByNotify()
{
	if (!GetCurrentInteractComponent()) return;
	EventInteract(GetCurrentInteractComponent(), CachedTargetInteractType);
}

void UBaseInteractManager::PressedTimerInteract(EInteractType Interact)
{
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(Timer_InteractPressed))
	{
		GetWorld()->GetTimerManager().ClearTimer(Timer_InteractPressed);
	}	
	if (!GetCurrentInteractComponent())	return;	

	if (GetCurrentInteractComponent()->HasInteractFlag(EInteractType::EIT_CirculPressed))
	{
		EventInteract(GetCurrentInteractComponent(), EInteractType::EIT_CirculPressed);
		GetCurrentInteractComponent()->StartInteract(GetOwningPlayerCharacter());
		
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, TEXT("PressedTimerInteract"), Interact);			
		GetWorld()->GetTimerManager().SetTimer(Timer_InteractPressed, Delegate, GetCurrentInteractComponent()->GetTimeToInteract(), false);

		return;
	}
	
	EventPreInteract(GetCurrentInteractComponent(), Interact);
}
#pragma endregion INPUT

#pragma region TRACE

void UBaseInteractManager::PerformTrace()
{
	if (!InteractManagerState.CanBeTrace()) return;

	TArray<FHitResult> LHitResults;
	FInteractDebugData DrawDebugData = FInteractDebugData::MakeDefault(false);
	FInteractTraceData TraceData(InteractProfile, InteractionSphereSize, GetCameraLocation(), DoCollisionTest(((MaxInteractDistance / 100) * 5)), GetOwningPlayerCharacter());
	
#if !UE_BUILD_SHIPPING
	DrawDebugData = FInteractDebugData::MakeDefault(CDrawDebugInteractedLine.GetValueOnGameThread());
#endif

	//Сначало проверить перед собой, иначе объемный трейс может не дать взаимодействовать при сложной геометрии 
	if (UInteractSystemLibrary::LineTrace(TraceData, LHitResults, DrawDebugData))
	{
		if (UBaseInteractComponent* LTargetInteractComponent = GetInteractionComponent(LHitResults, TraceData.EndVector))
		{
			NewInteraction(LTargetInteractComponent);
			return;
		}
	}
	
	LHitResults.Reset();
	if (!UInteractSystemLibrary::PerformTrace(TraceData, LHitResults, DrawDebugData))
	{
		InteractionNone();
		return;
	}	
	
	if (UBaseInteractComponent* LTargetInteractComponent = GetInteractionComponent(LHitResults, TraceData.EndVector))
	{
		NewInteraction(LTargetInteractComponent);
		return;
	}
	
	InteractionNone();
}

#pragma endregion TRACE

#pragma region INTERACT_STATE

void UBaseInteractManager::InteractionNone()
{
	if (!GetCurrentInteractComponent()) return;
	
	//Потенциально у нас может смениться LastInteractComponent при вызове OnInteractFocusLost или EndInteract, поэтому тут кешируется тот компонент с которым надо закончить работу
	UBaseInteractComponent* LLostInteractComponent = GetCurrentInteractComponent();
	InteractManagerState.SetNewInteractComponent(nullptr, nullptr);

	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(Timer_InteractPressed))
	{
		GetWorld()->GetTimerManager().ClearTimer(Timer_InteractPressed);
	}

	LLostInteractComponent->EndInteract(GetOwningPlayerCharacter());
	LLostInteractComponent->EndFocus(GetOwningPlayerCharacter());
	OnFocusLost.Broadcast(GetOwningPlayerCharacter(), LLostInteractComponent, LLostInteractComponent->GetOwner());
	
	if (!HasAuthority())
	{
		Server_EndFocus();
	}
}

void UBaseInteractManager::NewInteraction(UBaseInteractComponent* InteractComponent)
{
	if (!IsValid(InteractComponent)) return;
	if (GetCurrentInteractComponent() == InteractComponent) return;
	
	InteractionNone();
	
	if (!HasAuthority())
	{
		Server_BeginFocus(InteractComponent);
	}
	
	InteractManagerState.SetNewInteractComponent(InteractComponent, GetInteractAnimationComponent(InteractComponent));
	OnBeginFocus.Broadcast(GetOwningPlayerCharacter(), InteractComponent, InteractComponent->GetOwner());
	
	if (!GetCurrentInteractComponent()) return;

	GetCurrentInteractComponent()->BeginFocus(GetOwningPlayerCharacter());
}

#pragma endregion INTERACT_STATE


#pragma region TOOLS


UInteractAnimationComponent* UBaseInteractManager::GetInteractAnimationComponent(UBaseInteractComponent* From)
{
	if (!(IsValid(From) && From->GetOwner())) return nullptr;
	if (From->GetInteractAnimationComponent()) return From->GetInteractAnimationComponent();
	TArray<UInteractAnimationComponent*> Components;
	From->GetOwner()->GetComponents<UInteractAnimationComponent>(Components);

	UInteractAnimationComponent* Result = nullptr;
	for (auto Component : Components)
	{
		if (!Result)
		{
			Result = Component;
			continue;
		}
		float Distance_1 = FVector::Distance(From->GetComponentLocation(), Result->GetComponentLocation());
		float Distance_2 = FVector::Distance(From->GetComponentLocation(), Component->GetComponentLocation());
		if (Distance_2 < Distance_1)
		{
			Result = Component;
		}
	}

	return Result;
}

UBaseInteractComponent* UBaseInteractManager::GetInteractionComponent(TArray<FHitResult> Hits, FVector HitLocation,	bool IgnorePriority)
{
	float LDistance = 1000.f;
	UBaseInteractComponent* LPossibleInteractComponent = nullptr;

	TMap<UBaseInteractComponent*, FHitResult> Input;
	for (auto Hit : Hits)
	{
		AActor* TargetActor = Hit.GetActor();
		if (!IsValid(TargetActor)) continue;
		
		//Дистанция больше максимальной
		float Distance = FVector::Distance(GetOwningPlayerCharacter()->GetActorLocation(), TargetActor->GetActorLocation());
		if (Distance > MaxInteractDistance) continue;
	
		TArray<UBaseInteractComponent*> LInteractComponents;
		TargetActor->GetComponents<UBaseInteractComponent>(LInteractComponents);
	
		for (auto LInteractComponent : LInteractComponents)
		{
			if (!IsValid(LInteractComponent)) continue;
	
			Distance = FVector::Distance(GetOwningPlayerCharacter()->GetActorLocation(), LInteractComponent->GetComponentLocation());
			if (Distance > LInteractComponent->GetInteractSettings().InteractionDistance) continue;
	
			Input.Add(LInteractComponent, Hit);
		}
	}
	
	for (auto Pair : Input)
	{
		if (!LPossibleInteractComponent)
		{
			LPossibleInteractComponent = Pair.Key;
			LDistance = FVector::Distance(LPossibleInteractComponent->GetOwner()->GetActorLocation(), HitLocation);
			continue;
		}
		
		if (FVector::Distance(Pair.Key->GetOwner()->GetActorLocation(), HitLocation) < LDistance) // New = nearest ->  NewPossible
		{
			LDistance = FVector::Distance(Pair.Value.Location, HitLocation);
	
			LPossibleInteractComponent = Pair.Key;
		}
	}
	return LPossibleInteractComponent;
}

bool UBaseInteractManager::IsNeedMoving()
{
	if (!GetCurrentInteractComponent()) return false;
	if (!GetOwningPlayerCharacter()) return false;
	if (GetCurrentInteractAnimationComponent()->GetPositionType() == EInteractPositionType::EIPT_None) return false;
	
	return (!UKismetMathLibrary::EqualEqual_Vector2DVector2D(FVector2D(GetOwningPlayerCharacter()->GetActorLocation()), FVector2D(GetCurrentInteractAnimationComponent()->GetInteractTransform(false).GetLocation()), 5.f));
}

UBaseInteractManager* UBaseInteractManager::GetInteractManager(AActor* FromActor)
{
	if (!IsValid(FromActor)) return nullptr;
	return Cast<UBaseInteractManager>(FromActor->GetComponentByClass(UBaseInteractManager::StaticClass()));
}

AActor* UBaseInteractManager::GetInteractedActor()
{
	if (!(GetCurrentInteractComponent() && (GetCurrentInteractComponent() && GetCurrentInteractComponent()->GetOwner()))) return nullptr;

	if ((FVector::Distance(GetCurrentInteractComponent()->GetOwner()->GetActorLocation(), GetOwningPlayerCharacter()->GetActorLocation()) < (MaxInteractDistance + 100.f)))
	{
		return GetCurrentInteractComponent()->GetOwner();
	}
	else
	{
		InteractManagerState.SetNewInteractComponent(nullptr, nullptr);
		return nullptr;
	}
}

UCameraComponent* UBaseInteractManager::GetCameraComponent()
{
	if (PlayerCameraComponent) return PlayerCameraComponent;
	if (!GetOwningPlayerCharacter()) return nullptr;
	
	PlayerCameraComponent = Cast<UCameraComponent>(GetOwningPlayerCharacter()->GetComponentByClass(UCameraComponent::StaticClass()));
	return PlayerCameraComponent;
}

FVector UBaseInteractManager::GetCameraLocation()
{
	if (GetOwningPlayerController() && GetOwningPlayerController()->PlayerCameraManager)
	{
		return GetOwningPlayerController()->PlayerCameraManager->GetCameraLocation();
	}
	else if (GetCameraComponent())
	{
		return GetCameraComponent()->GetComponentLocation();
	}
	return FVector::ZeroVector;
}

FVector UBaseInteractManager::GetCameraForwardVector()
{
	if (GetOwningPlayerController() && GetOwningPlayerController()->PlayerCameraManager)
	{
		return GetOwningPlayerController()->PlayerCameraManager->GetCameraRotation().Vector();
	}
	if (GetCameraComponent())
	{
		return GetCameraComponent()->GetForwardVector();
	}
	return FVector::ZeroVector;
}

FVector UBaseInteractManager::GetEndVector()
{
	return (GetCameraLocation() + GetCameraForwardVector() * MaxInteractDistance);
}

bool UBaseInteractManager::DoCollisionTestHit(FHitResult& OutHitResult, float AppendVector)
{
	FCollisionQueryParams LCQP;
	LCQP.AddIgnoredActor(GetOwningPlayerCharacter());
	return GetWorld()->LineTraceSingleByChannel(OutHitResult, GetCameraLocation(), GetEndVector(), ECollisionChannel::ECC_Visibility, LCQP);
}

FVector UBaseInteractManager::DoCollisionTest(float AppendVector)
{
	FHitResult HitResult;
	bool LResult = DoCollisionTestHit(HitResult, AppendVector);
	return (LResult ? HitResult.Location : GetEndVector());
}

#pragma endregion TOOLS


#pragma region InteractStateMachine

void UBaseInteractManager::BreakCurrentInteraction()
{
	SwitchInteractState(ECharacterInteractState::ECIS_None);
}

bool UBaseInteractManager::SwitchInteractState(ECharacterInteractState NewState)
{
	InteractManagerState.OldInteractState = InteractManagerState.CurrentInteractState;
	InteractManagerState.CurrentInteractState = NewState;

	switch (NewState)
	{
	case ECharacterInteractState::ECIS_None:
	{
		if (InteractManagerState.OldInteractState == ECharacterInteractState::ECIS_MovingToInteractPoint) // В случае если было вызвано BreakCurrentInteraction
		{
			GetWorld()->GetTimerManager().ClearTimer(Timer_MoveTo);
		}
		if (InteractManagerState.OldInteractState == ECharacterInteractState::ECIS_InteractState && GetCurrentInteractComponent() && GetCurrentInteractAnimationComponent()->GetAnimationInteractRule()) // В случае если было вызвано BreakCurrentInteraction
		{
			GetCurrentInteractAnimationComponent()->GetAnimationInteractRule()->EndInteraction(GetCurrentInteractAnimationComponent(), GetOwningPlayerCharacter());
			GetOwner()->SetActorTransform(GetCurrentInteractAnimationComponent()->GetInteractTransform());
		}
		AnimationInteractSuccessfull(); //Состояние интеракта выключено
		InteractManagerState.TargetInteractState = ECharacterInteractState::ECIS_None;
		InteractManagerState.CurrentInteractStateTag = FGameplayTag();
		// OnChangeInteractStateType.Broadcast(this, InteractManagerState.CurrentInteractStateTag);
		return true;
	}
	case ECharacterInteractState::ECIS_MovingToInteractPoint:
	{
		return MoveToInteractPoint();
	}
	case ECharacterInteractState::ECIS_AwaitPlayerInput:
	{
		if (!GetCurrentInteractComponent()->GetOwner()) return false;
		return true;
	}
	case ECharacterInteractState::ECIS_InteractState:
	{
		if (GetCurrentInteractComponent() && GetCurrentInteractAnimationComponent()->GetAnimationInteractRule())
		{
			GetCurrentInteractAnimationComponent()->GetAnimationInteractRule()->OnInteracted.AddUniqueDynamic(this, &ThisClass::OnAnimationCompleted);
			InteractManagerState.CurrentInteractStateTag = GetCurrentInteractAnimationComponent()->GetInteractState();
			// OnChangeInteractStateType.Broadcast(this, InteractManagerState.CurrentInteractStateTag);
			GetCurrentInteractAnimationComponent()->GetAnimationInteractRule()->StartInteraction(GetCurrentInteractAnimationComponent(), GetOwningPlayerCharacter());
		}
		return true;
	}
	default:
		break;
	}
	return false;
}

void UBaseInteractManager::OnAnimationCompleted(ACharacter* InCharacterInstigator, UAnimationInteractRule* InInteractRules)
{
	if (InCharacterInstigator != GetOwner()) return;

	if (!GetCurrentInteractComponent()) return;
	EventInteract(GetCurrentInteractComponent(), CachedTargetInteractType);
}

bool UBaseInteractManager::MoveToInteractPoint()
{
	InteractManagerState.TargetInteractState = ECharacterInteractState::ECIS_InteractState;

	GetOwningPlayerCharacter()->GetCapsuleComponent()->SetCapsuleRadius(10.f);
	GetWorld()->GetTimerManager().SetTimer(Timer_MoveTo, this, &UBaseInteractManager::MoveTo, 0.01f, true);
	return true;
}

void UBaseInteractManager::AnimationInteractSuccessfull(){}

void UBaseInteractManager::NextInteractState() //Запустить следующие состояние State машины
{
	SwitchInteractState(InteractManagerState.TargetInteractState);
}


void UBaseInteractManager::MoveTo()
{
	if (!GetOwningPlayerCharacter() || !GetCurrentInteractComponent())
	{
		GetWorld()->GetTimerManager().ClearTimer(Timer_MoveTo);
		MoveEnd();
		return;
	}


	float LTolerance = (GetCurrentInteractAnimationComponent()->GetPositionType() == EInteractPositionType::EIPT_Point ? 15.f : GetCurrentInteractAnimationComponent()->GetInteractRadius());

	if (UKismetMathLibrary::EqualEqual_Vector2DVector2D(FVector2D(GetOwningPlayerCharacter()->GetActorLocation()), FVector2D(GetCurrentInteractAnimationComponent()->GetInteractTransform().GetLocation()), LTolerance))
	{
		FLatentActionInfo Callback;	Callback.CallbackTarget = this;	Callback.ExecutionFunction = FName("MoveEnd");	Callback.UUID = 0;	Callback.Linkage = 0;
		if (GetCurrentInteractAnimationComponent()->GetPositionType() == EInteractPositionType::EIPT_Point)
		{
			GetOwningPlayerCharacter()->GetMovementComponent()->Velocity = FVector::ZeroVector;
			UKismetSystemLibrary::MoveComponentTo(GetOwningPlayerCharacter()->GetCapsuleComponent(), GetCurrentInteractAnimationComponent()->GetInteractTransform(false).GetLocation(), GetCurrentInteractAnimationComponent()->GetInteractTransform(false).GetRotation().Rotator(), false, false, 0.25f, false, EMoveComponentAction::Move, Callback);
		}
		else
		{
			MoveEnd();
		}
		GetWorld()->GetTimerManager().ClearTimer(Timer_MoveTo);

		return;
	}
	FRotator Direction = UKismetMathLibrary::FindLookAtRotation(GetOwningPlayerCharacter()->GetActorLocation(), GetCurrentInteractAnimationComponent()->GetInteractTransform(false).GetLocation());
	GetOwningPlayerCharacter()->GetCharacterMovement()->AddInputVector(Direction.Vector());
}

void UBaseInteractManager::MoveEnd()
{
	SwitchInteractState(InteractManagerState.TargetInteractState);
}

#pragma endregion


void UBaseInteractManager::OnRep_InteractManagerState()
{
	if (InteractManagerState.CurrentInteractState == ECharacterInteractState::ECIS_InteractState)
	{
		SwitchInteractState(InteractManagerState.CurrentInteractState);
	}
	// OnChangeInteractStateType.Broadcast(this, InteractManagerState.CurrentInteractStateTag);
}