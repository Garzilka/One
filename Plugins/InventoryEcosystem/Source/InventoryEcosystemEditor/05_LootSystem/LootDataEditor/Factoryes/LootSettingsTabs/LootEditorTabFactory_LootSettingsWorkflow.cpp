#include "LootEditorTabFactory_LootSettingsWorkflow.h"
#include "EditorStyleSet.h"
#include "InventoryEcosystemEditor/05_LootSystem/LootDataEditor/LootEditorAssetEditor.h"


FLootEditorTabFactory_LootSettingsWorkflow::FLootEditorTabFactory_LootSettingsWorkflow(TSharedRef<FLootEditorAssetEditor> InLootEditor)
	:FWorkflowTabFactory(LootEditorTabs::LootSettings_Workflow, InLootEditor), LootAssetEditor(InLootEditor)
{
	TabLabel = FText::FromString(TEXT("Workflow"));
	TabIcon = FSlateIcon(FEditorStyle::GetStyleSetName(), "ContentReference.PickAsset");
	bIsSingleton = true;
}

TSharedRef<SWidget> FLootEditorTabFactory_LootSettingsWorkflow::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	if (!LootAssetEditor.IsValid()) return SNew(STextBlock).Text(FText::FromString("Error Tab"));
	return SNew(STextBlock).Text(FText::FromString("Loot settings workflow"));
}

FText FLootEditorTabFactory_LootSettingsWorkflow::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FWorkflowTabFactory::GetTabToolTipText(Info);
}