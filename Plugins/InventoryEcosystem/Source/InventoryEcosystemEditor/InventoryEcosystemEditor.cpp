// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryEcosystemEditor.h"

#include "02_InteractSystemCustomiztion/InteractSeetingsCustomization/InteractSeetingsCustomization.h"
#include "InventoryEcosystem/02_InteractSystem/Data/InteractData.h"

#define LOCTEXT_NAMESPACE "FInventoryEcosystemEditorModule"

void FInventoryEcosystemEditorModule::StartupModule()
{
	
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	// to register our custom property
	PropertyModule.RegisterCustomPropertyTypeLayout(FInteractSettings::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FInteractSeetingsCustomization::MakeInstance));
	
	PropertyModule.NotifyCustomizationModuleChanged();
}

void FInventoryEcosystemEditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FInventoryEcosystemEditorModule, InventoryEcosystemEditor)