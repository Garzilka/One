#include "SimpleAnimatedInteract.h"

#include "InventoryEcosystem/02_InteractSystem/Component/InteractAnimationComponent.h"

void USimpleAnimatedInteract::StartInteraction(ACharacter* TargetPlayer)
{	
	if (!IsValid(TargetPlayer)) return;
	
	OnStartAnimation.Broadcast(TargetPlayer, this);
	RunInteract(TPSInteractAnimMontage, TargetPlayer);
	CancelInteract(TPSInteractAnimMontage, TargetPlayer);
	
	if (!TargetPlayer->GetMesh()) return;
	if (!TargetPlayer->GetMesh()->GetAnimInstance()) return;
	TargetPlayer->GetMesh()->GetAnimInstance()->Montage_Play(TPSInteractAnimMontage);
}
