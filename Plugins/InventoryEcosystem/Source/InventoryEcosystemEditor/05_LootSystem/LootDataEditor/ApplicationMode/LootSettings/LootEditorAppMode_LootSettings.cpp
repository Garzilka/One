#include "LootEditorAppMode_LootSettings.h"

#include "InventoryEcosystemEditor/05_LootSystem/LootDataEditor/LootEditorAssetEditor.h"
#include "InventoryEcosystemEditor/05_LootSystem/LootDataEditor/Factoryes/LootSettingsTabs/LootEditorTabFactory_LootSettingsList.h"
#include "InventoryEcosystemEditor/05_LootSystem/LootDataEditor/Factoryes/LootSettingsTabs/LootEditorTabFactory_LootSettingsWorkflow.h"


FLootEditorAppMode_LootSettings::FLootEditorAppMode_LootSettings(TSharedPtr<FLootEditorAssetEditor> InLootEditor, FName InApplicationMode, TSharedPtr<FTabManager::FLayout> InLayout)
: FApplicationMode(InApplicationMode)
{
	LootEditor = InLootEditor;

	LootEditorTabFactories.RegisterFactory(MakeShareable(new FLootEditorTabFactory_LootSettingsList(InLootEditor.ToSharedRef())));
	LootEditorTabFactories.RegisterFactory(MakeShareable(new FLootEditorTabFactory_LootSettingsWorkflow(InLootEditor.ToSharedRef())));

	TabLayout = InLayout;
}

void FLootEditorAppMode_LootSettings::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	check(LootEditor.IsValid());
	TSharedPtr<FLootEditorAssetEditor> LootEditorAssetEditorPtr = LootEditor.Pin();
	LootEditorAssetEditorPtr->RegisterToolBarTab(InTabManager.ToSharedRef());
	LootEditorAssetEditorPtr->PushTabFactories(LootEditorTabFactories);
	FApplicationMode::RegisterTabFactories(InTabManager);
}