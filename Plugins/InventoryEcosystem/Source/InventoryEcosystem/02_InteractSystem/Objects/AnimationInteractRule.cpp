#include "AnimationInteractRule.h"


void UAnimationInteractRule::InteractCallback(ACharacter* Target)
{
	if (!(IsPossiblyAllocatedUObjectPointer(Target) && IsValid(Target))) return;
	
	if (InteractCallType == EInteractCallType::EICT_BeforeInteractAnimation)
	{
		CurrentInteractions.Remove(Target);
	}
	
	OnInteracted.Broadcast(Target, this);
}

void UAnimationInteractRule::RunInteract(float Time, UInteractAnimationComponent* Component, ACharacter* TargetPlayer)
{
	if (InteractCallType == EInteractCallType::EICT_BeforeInteractAnimation)
	{
		InteractCallback(TargetPlayer);
	}

	if (InteractCallType == EInteractCallType::EICT_AfterInteractAnimation)
	{
		FTimerHandle Timer_Broadcast;
		
		if (auto TimerRef = CurrentInteractions.Find(TargetPlayer))
		{
			Timer_Broadcast = *TimerRef;
			GetWorld()->GetTimerManager().ClearTimer(Timer_Broadcast);
		}
		else
		{
			CurrentInteractions.Add(TargetPlayer, Timer_Broadcast);
		}
			
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, TEXT("InteractCallback"), TargetPlayer);			
		GetWorld()->GetTimerManager().SetTimer(Timer_Broadcast, Delegate, Time, false);
	}
}
