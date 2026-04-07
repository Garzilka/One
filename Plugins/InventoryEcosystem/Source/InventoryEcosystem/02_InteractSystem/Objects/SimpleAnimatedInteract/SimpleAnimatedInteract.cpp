#include "SimpleAnimatedInteract.h"

#include "InventoryEcosystem/02_InteractSystem/Component/InteractAnimationComponent.h"

void USimpleAnimatedInteract::StartInteraction(UInteractAnimationComponent* Component, ACharacter* TargetPlayer)
{	
	if (!IsValid(TargetPlayer) || !IsValid(Component)) return;
	if (!TargetPlayer->GetMesh()) return;
	if (!TargetPlayer->GetMesh()->GetAnimInstance()) return;
	
	OnStartAnimation.Broadcast(TargetPlayer, this);
	RunInteract(TargetPlayer->GetMesh()->GetAnimInstance()->Montage_Play(TPSInteractAnimMontage), Component, TargetPlayer);
}
