// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractAnimationComponent.h"

#include "InventoryEcosystem/01_Core/Library/InventoryCoreLibrary.h"
#include "InventoryEcosystem/02_InteractSystem/InteractSystemSettings.h"
#include "InventoryEcosystem/02_InteractSystem/Objects/AnimationInteractRule.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UInteractAnimationComponent::UInteractAnimationComponent()
{
	SetHiddenInGame(true);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetAnimationMode(EAnimationMode::AnimationSingleNode);
	if (const UInteractSystemSettings* InteractSystemSettings = UInteractSystemSettings::Get())
	{
		if (USkeletalMesh* Mesh = InteractSystemSettings->PreviewInteractMesh.LoadSynchronous())
		{
			SetSkeletalMesh(Mesh);
		}
	}

	AnimationData.bSavedPlaying = false;
	AnimationData.bSavedLooping = false;
	
	if (AnimationRule && AnimationRule->GetPreviewAnimation())
	{
		UAnimMontage* Montage = AnimationRule->GetPreviewAnimation();
		AnimationData.AnimToPlay = Montage;
		AnimationData.SavedPosition = AnimationRule->PreviewTime;
		PlayAnimation(AnimationData.AnimToPlay, false);
		SetPosition(AnimationData.SavedPosition, false);
	}
}


void UInteractAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

#if WITH_EDITOR
	if (GetWorld() && GetPositionType() == EInteractPositionType::EIPT_Radius)
	{
		if ((GetWorld()->WorldType == EWorldType::Editor && !UInventoryCoreLibrary::IsInGameView()) || GetWorld()->WorldType == EWorldType::EditorPreview)
		{
			int32 NumSec = 24;
		
			FRotator Rot = FRotator(0.f, 360, 0.f);		
			UKismetSystemLibrary::DrawDebugConeInDegrees(GetWorld(), GetComponentLocation(), Rot.RotateVector(GetForwardVector()), GetInteractRadius(), 360.f, 0.f, NumSec, FColor::Orange, 0.f, 1.f);
		}
	}
#endif
}

FTransform UInteractAnimationComponent::GetInteractTransform(bool IsRelative)
{
	if (IsRelative) return GetRelativeTransform();

	FTransform Result;
	FTransform ActorTrnsf = GetOwner()->GetActorTransform();

	FVector LLocation = GetRelativeTransform().GetLocation();
	FRotator LRotation = FRotator(GetRelativeTransform().GetRotation());
	
	Result.SetLocation(UKismetMathLibrary::TransformLocation(ActorTrnsf, LLocation));
	Result.SetRotation(UKismetMathLibrary::TransformRotation(ActorTrnsf, LRotation).Quaternion());

	return Result;
}

void UInteractAnimationComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (AnimationRule)
	{
		AnimationRule->OnInteracted.AddUniqueDynamic(this, &ThisClass::Event_Interacted);
		AnimationRule->OnStartAnimation.AddUniqueDynamic(this, &ThisClass::Event_StartAnimation);
		AnimationRule->OnEndAnimation.AddUniqueDynamic(this, &ThisClass::Event_EndAnimation);
		AnimationRule->OnInteractAnimation.AddUniqueDynamic(this, &ThisClass::Event_InteractAnimation);
	}
}

void UInteractAnimationComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	SetVisibleFlag(PositionType == EInteractPositionType::EIPT_Point);
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (AnimationRule && AnimationRule->GetPreviewAnimation())
	{
		UAnimMontage* Montage = AnimationRule->GetPreviewAnimation();
		AnimationData.AnimToPlay = Montage;
		AnimationData.SavedPosition = AnimationRule->PreviewTime;
		PlayAnimation(AnimationData.AnimToPlay, false);
		SetPosition(AnimationData.SavedPosition, false);
	}
}

void UInteractAnimationComponent::Event_StartAnimation(ACharacter* CharacterInstigator,	UAnimationInteractRule* InteractRules)
{
	OnStartAnimation.Broadcast(CharacterInstigator, this, InteractRules);
};

void UInteractAnimationComponent::Event_Interacted(ACharacter* CharacterInstigator,	UAnimationInteractRule* InteractRules)
{
	OnInteracted.Broadcast(CharacterInstigator, this, InteractRules);
};

void UInteractAnimationComponent::Event_EndAnimation(ACharacter* CharacterInstigator, UAnimationInteractRule* InteractRules)
{
	OnStartAnimation.Broadcast(CharacterInstigator, this, InteractRules);
};

void UInteractAnimationComponent::Event_InteractAnimation(ACharacter* CharacterInstigator, UAnimationInteractRule* InteractRules)
{
	OnStartAnimation.Broadcast(CharacterInstigator, this, InteractRules);
};
