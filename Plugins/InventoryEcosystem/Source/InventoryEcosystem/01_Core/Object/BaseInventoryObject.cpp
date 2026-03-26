#include "BaseInventoryObject.h"
#include "InventoryEcosystem/01_Core/Interface/ObjectManagmentInterface.h"
#include "InventoryEcosystem/01_Core/Library/InventoryCoreLibrary.h"


#pragma region ENGINE

void UBaseInventoryObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseInventoryObject, RepOuter);
	DOREPLIFETIME(UBaseInventoryObject, ParrentObject);
}

bool UBaseInventoryObject::CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack)
{
	if (!GetOwner()) return false;
	
	UNetDriver* NetDriver = GetOwner()->GetNetDriver();
	if (!NetDriver) return false;
	
	NetDriver->ProcessRemoteFunction(GetOwner(), Function, Parms, OutParms, Stack, this);
	return true;
}

int32 UBaseInventoryObject::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	return (GetOuter() ? GetOuter()->GetFunctionCallspace(Function, Stack) : FunctionCallspace::Local);
}

#pragma endregion

#pragma region RENAME 

bool UBaseInventoryObject::Rename(const TCHAR* NewName, UObject* NewOuter, ERenameFlags Flags)
{
	CharacterOwner = nullptr;
	ControllerOwner = nullptr;

	if (HasAuthority())
	{
		RepOuter = NewOuter;
	}
	return Super::Rename(NewName, NewOuter, Flags);
}

void UBaseInventoryObject::OnRep_Outer()
{
	if (RepOuter == nullptr) return;

	if (!HasAuthority())
	{
		Rename(nullptr, RepOuter);
	}
}

#pragma endregion RENAME

#pragma region TOOLS

AActor* UBaseInventoryObject::GetOwner()
{
	if (Owner != GetOuter())
	{
		Owner = Cast<AActor>(GetOuter());
	}
	return Owner;
}

APlayerController* UBaseInventoryObject::GetOwningPlayer()
{
	if (IsValid(ControllerOwner)) return ControllerOwner;

	ControllerOwner = Cast<APlayerController>(GetOwner());

	if (!ControllerOwner && GetOwningPlayerCharacter())
	{
		ControllerOwner = Cast<APlayerController>(GetOwningPlayerCharacter()->GetController());
	}

	return ControllerOwner;
}

ACharacter* UBaseInventoryObject::GetOwningPlayerCharacter()
{
	if (IsValid(CharacterOwner)) return CharacterOwner;

	CharacterOwner = Cast<ACharacter>(GetOwner());
	if (!CharacterOwner && GetOwningPlayer())
	{
		CharacterOwner = Cast<ACharacter>(GetOwningPlayer()->GetPawn());
	}
	return CharacterOwner;
}

bool UBaseInventoryObject::SwitchParrent(UObject* NewParrent)
{
	if (IsParrentEqual(NewParrent)) return true;
	if (ParrentObject)
	{
		IObjectManagmentInterface* ManageInterface = UInventoryCoreLibrary::HasManagmentInterface(ParrentObject);
		if (!ensure(ManageInterface))
		{
			LOG_BIO(E_IES_LOG_TYPE::EIEST_Error, "Error: The inventory system cannot remove the item from the owner. The owner does not have a product management interface\n Transfer refused");
			return false;
		}
		if (!ManageInterface->Execute_I_RemoveObject(ParrentObject, this))
		{
			LOG_BIO(E_IES_LOG_TYPE::EIEST_Warning, "Warning: The inventory system cannot remove the item from the owner. The RemoveItem() function returns false. Transfer refused");
			return false;
		}
	}
	else
	{
		LOG_BIO(E_IES_LOG_TYPE::EIEST_Warning, "Warning: ItemOwner == nullptr");
	}
	ParrentObject = NewParrent;

	if (ParrentObject)
	{
		IObjectManagmentInterface* ManageInterface = UInventoryCoreLibrary::HasManagmentInterface(ParrentObject);
		if (!ensure(ManageInterface))
		{
			LOG_BIO(E_IES_LOG_TYPE::EIEST_Error, "Error: The new owner does not have a product management interface");
			return true;
		}
	}

	return true;
}

bool UBaseInventoryObject::RemoveFromParrent()
{
	if (ParrentObject)
	{
		IObjectManagmentInterface* ManageInterface = UInventoryCoreLibrary::HasManagmentInterface(ParrentObject);
		if (!ensure(ManageInterface))
		{
			LOG_BIO(E_IES_LOG_TYPE::EIEST_Error, "Error: The inventory system cannot remove the item from the owner. The owner does not have a product management interface\n Transfer refused");
			return false;
		}
		if (!ManageInterface->Execute_I_RemoveObject(ParrentObject, this))
		{
			LOG_BIO(E_IES_LOG_TYPE::EIEST_Warning, "Warning: The inventory system cannot remove the item from the owner. The RemoveItem() function returns false. Transfer refused");
			return false;
		}
	}
	else
	{
		LOG_BIO(E_IES_LOG_TYPE::EIEST_Warning, "Warning: ItemOwner == nullptr");
	}
	return true;
}

bool UBaseInventoryObject::IsCanRemoveFromParrent()
{
	if (!ParrentObject) return true;
	if (ParrentObject)
	{
		IObjectManagmentInterface* ManageInterface = UInventoryCoreLibrary::HasManagmentInterface(ParrentObject);
		if (!ensure(ManageInterface))
		{
			LOG_BIO(E_IES_LOG_TYPE::EIEST_Error, "Error: The inventory system cannot remove the item from the owner. The owner does not have a product management interface\n Transfer refused");
			return false;
		}
		return ManageInterface->Execute_I_RemoveObject(ParrentObject, this);
	}
	return false;
}

void UBaseInventoryObject::ChangeOwner(AActor* InNewOuter)
{
	if (!(IsValid(InNewOuter) && InNewOuter->HasAuthority())) return;
	if (InNewOuter == GetOuter())
	{
		if (RepOuter != InNewOuter) RepOuter = InNewOuter;
		return;
	}

	//Change Outer
	Rename(nullptr, InNewOuter);
}

#pragma endregion TOOLS

