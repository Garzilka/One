#include "AnimationInteractRule.h"

#include "InventoryEcosystem/02_InteractSystem/AnimNotify/AnimNotify_Interacted.h"


void UAnimationInteractRule::InteractCallback(ACharacter* Target)
{
	if (!(IsPossiblyAllocatedUObjectPointer(Target) && IsValid(Target))) return;
	OnInteracted.Broadcast(Target, this);
}

void UAnimationInteractRule::RunInteract(UAnimMontage* Montage, ACharacter* TargetPlayer)
{
	if (!Montage)
	{
		InteractCallback(TargetPlayer);
		return;
	}
	
	FName NameNotify = FName(UAnimNotify_Interacted::StaticClass()->GetName());
	float LTime = Montage->GetPlayLength();
	for (auto notify : Montage->Notifies)
	{
		if (notify.NotifyName != NameNotify) continue;
		
		LTime = (1.f/Montage->RateScale) * notify.GetTriggerTime();
	}
	
	GetWorld()->GetTimerManager().ClearTimer(Timer_TimeToInteract);
	
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, TEXT("InteractCallback"), TargetPlayer);			
	GetWorld()->GetTimerManager().SetTimer(Timer_TimeToInteract, Delegate, LTime, false);
}

void UAnimationInteractRule::CancelInteractCallback(ACharacter* Target)
{
	if (!(IsPossiblyAllocatedUObjectPointer(Target) && IsValid(Target))) return;
	OnCancelInteract.Broadcast(Target, this);
}

void UAnimationInteractRule::CancelInteract(UAnimMontage* Montage,	ACharacter* TargetPlayer)
{
	if (!Montage)
	{
		CancelInteractCallback(TargetPlayer);
		return;
	}
	GetWorld()->GetTimerManager().ClearTimer(Timer_CancelInteract);
	
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, TEXT("CancelInteractCallback"), TargetPlayer);			
	GetWorld()->GetTimerManager().SetTimer(Timer_CancelInteract, Delegate, Montage->GetPlayLength(), false);
}
