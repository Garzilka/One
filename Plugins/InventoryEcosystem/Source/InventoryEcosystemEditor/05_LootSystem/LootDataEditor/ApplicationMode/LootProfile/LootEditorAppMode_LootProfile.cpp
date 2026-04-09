#include "LootEditorAppMode_LootProfile.h"

#include "InventoryEcosystemEditor/05_LootSystem/LootDataEditor/LootEditorAssetEditor.h"
#include "InventoryEcosystemEditor/05_LootSystem/LootDataEditor/Factoryes/LootProfile/LootEditorTabFactory_LootProfileList.h"
#include "InventoryEcosystemEditor/05_LootSystem/LootDataEditor/Factoryes/LootProfile/LootEditorTabFactory_LootProfileWorkflow.h"


FLootEditorAppMode_LootProfile::FLootEditorAppMode_LootProfile(TSharedPtr<FLootEditorAssetEditor> InLootEditor, FName InApplicationMode, TSharedPtr<FTabManager::FLayout> InLayout)
: FApplicationMode(InApplicationMode)
{
	LootEditor = InLootEditor;

	LootEditorTabFactories.RegisterFactory(MakeShareable(new FLootEditorTabFactory_LootProfileList(InLootEditor.ToSharedRef())));
	LootEditorTabFactories.RegisterFactory(MakeShareable(new FLootEditorTabFactory_LootProfileWorkflow(InLootEditor.ToSharedRef())));

	TabLayout = InLayout;
}

void FLootEditorAppMode_LootProfile::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	check(LootEditor.IsValid());
	TSharedPtr<FLootEditorAssetEditor> LootEditorAssetEditorPtr = LootEditor.Pin();
	LootEditorAssetEditorPtr->RegisterToolBarTab(InTabManager.ToSharedRef());
	LootEditorAssetEditorPtr->PushTabFactories(LootEditorTabFactories);
	FApplicationMode::RegisterTabFactories(InTabManager);
}
