#include "InOutInteract.h"
#include "GameFramework/Character.h"
#include "InventoryEcosystem/02_InteractSystem/Component/InteractAnimationComponent.h"

void UInOutInteract::StartInteraction(ACharacter* TargetPlayer)
{
	if (!IsValid(TargetPlayer)) return;
	
	RunInteract(TPSBeginAnimMontage, TargetPlayer);
	OnStartAnimation.Broadcast(TargetPlayer, this);
	
	if (!TargetPlayer->GetMesh()) return;
	if (!TargetPlayer->GetMesh()->GetAnimInstance()) return;
	
	TargetPlayer->GetMesh()->GetAnimInstance()->Montage_Play(TPSBeginAnimMontage);
}

void UInOutInteract::EndInteraction(ACharacter* TargetPlayer)
{
	if (!IsValid(TargetPlayer)) return;
	
	CancelInteract(TPSEndAnimMontage, TargetPlayer);
	OnEndAnimation.Broadcast(TargetPlayer, this);
	
	if (!TargetPlayer->GetMesh()) return;
	if (!TargetPlayer->GetMesh()->GetAnimInstance()) return;
	
	TargetPlayer->GetMesh()->GetAnimInstance()->Montage_Play(TPSEndAnimMontage);
}