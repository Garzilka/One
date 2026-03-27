// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayLibrary.h"


AGameplayHUD* UGameplayLibrary::GetGameplayHUD(const UObject* WorldContextObject)
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!Controller) return nullptr;
	if (!Controller->GetHUD()) return nullptr;
	
	return Cast<AGameplayHUD>(Controller->GetHUD());
}

void UGameplayLibrary::SetUIState(const UObject* WorldContextObject, EUIState NewState)
{
	if (!WorldContextObject) return;
	
	AGameplayHUD* LHUD = GetGameplayHUD(WorldContextObject);
	if (!LHUD) return;
	
	LHUD->SetUIState(NewState);
}
