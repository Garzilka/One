#include "AnimatedInteraction.h"
#include "InventoryEcosystem/02_InteractSystem/Component/InteractAnimationComponent.h"


void UAnimatedInteraction::StartInteraction(UInteractAnimationComponent* Component, ACharacter* TargetPlayer)
{
	if (!IsValid(TargetPlayer) || !IsValid(Component)) return;
	if (!TargetPlayer->GetMesh()) return;
	if (!TargetPlayer->GetMesh()->GetAnimInstance()) return;
	
	OnStartAnimation.Broadcast(TargetPlayer, this);
	RunInteract(TargetPlayer->GetMesh()->GetAnimInstance()->Montage_Play(TPSBeginAnimMontage), Component, TargetPlayer);
}

void UAnimatedInteraction::EndInteraction(UInteractAnimationComponent* Component, ACharacter* TargetPlayer)
{
	if (!IsValid(TargetPlayer) || !IsValid(Component)) return;
	if (!TargetPlayer->GetMesh()) return;
	if (!TargetPlayer->GetMesh()->GetAnimInstance()) return;

	OnEndAnimation.Broadcast(TargetPlayer, this);
	TargetPlayer->GetMesh()->GetAnimInstance()->Montage_Play(TPSEndAnimMontage);
}

void UAnimatedInteraction::DoInteraction(UInteractAnimationComponent* Component, ACharacter* TargetPlayer)
{
	if (!IsValid(TargetPlayer) || !IsValid(Component)) return;
	if (!TargetPlayer->GetMesh()) return;
	if (!TargetPlayer->GetMesh()->GetAnimInstance()) return;

	OnInteractAnimation.Broadcast(TargetPlayer, this);
	TargetPlayer->GetMesh()->GetAnimInstance()->Montage_Play(TPSInteractAnimMontage);
}