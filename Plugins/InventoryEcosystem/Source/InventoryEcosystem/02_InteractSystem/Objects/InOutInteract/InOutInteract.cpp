#include "InOutInteract.h"
#include "GameFramework/Character.h"
#include "InventoryEcosystem/02_InteractSystem/Component/InteractAnimationComponent.h"

void UInOutInteract::StartInteraction(UInteractAnimationComponent* Component, ACharacter* TargetPlayer)
{
	if (!IsValid(TargetPlayer) || !IsValid(Component)) return;
	if (!TargetPlayer->GetMesh()) return;
	if (!TargetPlayer->GetMesh()->GetAnimInstance()) return;
	
	OnStartAnimation.Broadcast(TargetPlayer, this);
	RunInteract(TargetPlayer->GetMesh()->GetAnimInstance()->Montage_Play(TPSBeginAnimMontage), Component, TargetPlayer);
}

void UInOutInteract::EndInteraction(UInteractAnimationComponent* Component, ACharacter* TargetPlayer)
{
	if (!IsValid(TargetPlayer) || !IsValid(Component)) return;
	if (!TargetPlayer->GetMesh()) return;
	if (!TargetPlayer->GetMesh()->GetAnimInstance()) return;
	
	OnStartAnimation.Broadcast(TargetPlayer, this);
	RunInteract(TargetPlayer->GetMesh()->GetAnimInstance()->Montage_Play(TPSEndAnimMontage), Component, TargetPlayer);
}