#include "InventoryCoreLogTools.h"

#include "GameFramework/PlayerState.h"
#include "InventoryEcosystem/01_Core/Library/InventoryCoreLibrary.h"
#include "InventoryEcosystem/01_Core/Object/BaseInventoryObject.h"


DEFINE_LOG_CATEGORY_STATIC(LOG_IES_BIO, All, All);
static TAutoConsoleVariable<bool> C_IES_VIEW_LOG_BIO(TEXT("IES.LOG.BIO"), false, TEXT("Enable write log to screen"), ECVF_Cheat);

void UInventoryCoreLogTools::LOG(AActor* Owner, UBaseInventoryObject* InObject, E_IES_LOG_TYPE TypeLog, FString Str, int Line, const char* Function)
{
	if (Owner == nullptr || InObject == nullptr) return;

	if (C_IES_VIEW_LOG_BIO.GetValueOnGameThread())
	{
		UInventoryCoreLogTools::PrintString(Str, UInventoryCoreLogTools::ErrorToColor(TypeLog));
	}

	switch (TypeLog)
	{
	case E_IES_LOG_TYPE::EIEST_Display:
		{
			UE_LOG(LOG_IES_BIO, Display, TEXT("[%s][%i] | Net mode: %s | Player name: %s | Item name: %s | Text: %s"),
				ANSI_TO_TCHAR(Function), Line, *UInventoryCoreLogTools::GetNetMode(Owner), *UInventoryCoreLogTools::GetPlayerNickname(Owner), *(InObject->GetClass()->GetName()), *Str);
			break;
		}
	case E_IES_LOG_TYPE::EIEST_Warning:
		UE_LOG(LOG_IES_BIO, Warning, TEXT("[%s][%i] | Net mode: %s | Player name: %s | Item name: %s | Text: %s"),
			ANSI_TO_TCHAR(Function), Line, *UInventoryCoreLogTools::GetNetMode(Owner), *UInventoryCoreLogTools::GetPlayerNickname(Owner), *(InObject->GetClass()->GetName()), *Str);
		break;
	case E_IES_LOG_TYPE::EIEST_Error:
		UE_LOG(LOG_IES_BIO, Error, TEXT("[%s][%i] | Net mode: %s | Player name: %s | Item name: %s | Text: %s"),
			ANSI_TO_TCHAR(Function), Line, *UInventoryCoreLogTools::GetNetMode(Owner), *UInventoryCoreLogTools::GetPlayerNickname(Owner), *(InObject->GetClass()->GetName()), *Str);
		break;
	default:
		break;
	}
}

void UInventoryCoreLogTools::PrintString(FString Message, FColor Color)
{
	if (!GEngine) return;
	GEngine->AddOnScreenDebugMessage(-1, 8, Color, Message);
}

FColor UInventoryCoreLogTools::ErrorToColor(E_IES_LOG_TYPE ErrorType)
{
	switch (ErrorType)
	{
	case E_IES_LOG_TYPE::EIEST_Display:	return FColor::White;
	case E_IES_LOG_TYPE::EIEST_Warning:	return FColor::Orange;
	case E_IES_LOG_TYPE::EIEST_Error:	return FColor::Red;
	default:								return FColor();
	}
}

FString UInventoryCoreLogTools::GetPlayerNickname(AActor* From)
{
	if (!From) return "";
	APlayerState* LPlayerState = UInventoryCoreLibrary::GetPlayerState(From);
	if (!LPlayerState) return From->GetName();
	return LPlayerState->GetPlayerName();
}

FString UInventoryCoreLogTools::GetNetMode(AActor* InActor)
{
	if (!InActor) return FString("nullptr");

	const ENetMode LNetMode = InActor->GetNetMode();

	switch (LNetMode)
	{
	case NM_Standalone:			return FString("NM_Standalone");
	case NM_DedicatedServer:	return FString("NM_DedicatedServer");
	case NM_Client:				return FString("NM_Client");
	case NM_ListenServer:		return FString("NM_ListenServer");
	default: return FString("nullptr");
	}
}
