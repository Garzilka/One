// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryEcosystemEditor.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "02_InteractSystemCustomiztion/InteractSeetingsCustomization/InteractSeetingsCustomization.h"
#include "05_LootSystem/LootDataEditor/Factoryes/LootEditor_AssetFactory.h"
#include "InventoryEcosystem/02_InteractSystem/Data/InteractData.h"

#define LOCTEXT_NAMESPACE "FInventoryEcosystemEditorModule"

EAssetTypeCategories::Type  FInventoryEcosystemEditorModule::InventoryEcosystemCategory = static_cast<EAssetTypeCategories::Type>(0);

void FInventoryEcosystemEditorModule::StartupModule()
{	
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	// to register our custom property
	PropertyModule.RegisterCustomPropertyTypeLayout(FInteractSettings::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FInteractSeetingsCustomization::MakeInstance));
	
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	InventoryEcosystemCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("InventoryEcosystem")), FText::FromString(TEXT("InventoryEcosystem")));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_LootEditor_AssetAction()));
	
	PropertyModule.NotifyCustomizationModuleChanged();
}

void FInventoryEcosystemEditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FInventoryEcosystemEditorModule, InventoryEcosystemEditor)