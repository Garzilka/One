#include "InventoryCoreLibrary.h"

#include "InventoryEcosystem/01_Core/Interface/ObjectManagmentInterface.h"
#include "Kismet/KismetSystemLibrary.h"

IObjectManagmentInterface* UInventoryCoreLibrary::HasManagmentInterface(UObject* TargetObject)
{
	if (TargetObject == nullptr) return nullptr;
	if (UKismetSystemLibrary::DoesImplementInterface(TargetObject, UObjectManagmentInterface::StaticClass()))
	{
		return Cast<IObjectManagmentInterface>(TargetObject);
	}
	return nullptr;
}

APlayerState* UInventoryCoreLibrary::GetPlayerState(AActor* Owner)
{
	return (Cast<ACharacter>(Owner) != nullptr ? GetPlayerState(Cast<ACharacter>(Owner)) : GetPlayerState(Cast<APlayerController>(Owner)));
}

APlayerState* UInventoryCoreLibrary::GetPlayerState(ACharacter* Owner)
{
	if (!Owner) return nullptr;
	APlayerState* LResult = Owner->GetPlayerState();
	if (LResult)
	{
		return LResult;
	}
	else if (Owner->GetController())
	{
		APlayerController* LPlayerController = Cast<APlayerController>(Owner->GetController());
		if (!LPlayerController) return LResult;
		return LPlayerController->GetPlayerState<APlayerState>();
	}
	return nullptr;
}

APlayerState* UInventoryCoreLibrary::GetPlayerState(APlayerController* Owner)
{
	if (!Owner) return nullptr;
	APlayerState* LResult = Owner->GetPlayerState<APlayerState>();
	if (LResult)
	{
		return LResult;
	}
	else if (Owner->GetPawn())
	{
		ACharacter* LPlayerCharacter = Cast<ACharacter>(Owner->GetPawn());
		if (!LPlayerCharacter) return LResult;
		return LPlayerCharacter->GetPlayerState();
	}
	return nullptr;
}