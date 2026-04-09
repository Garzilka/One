#include "LootEditorTabFactory_LootProfileWorkflow.h"
#include "EditorStyleSet.h"
#include "InventoryEcosystemEditor/05_LootSystem/LootDataEditor/LootEditorAssetEditor.h"


FLootEditorTabFactory_LootProfileWorkflow::FLootEditorTabFactory_LootProfileWorkflow(TSharedRef<FLootEditorAssetEditor> InLootEditor)
	:FWorkflowTabFactory(LootEditorTabs::LootProfile_Workflow, InLootEditor), LootAssetEditor(InLootEditor)
{
	TabLabel = FText::FromString(TEXT("Workflow"));
	TabIcon = FSlateIcon(FEditorStyle::GetStyleSetName(), "ContentReference.PickAsset");
	bIsSingleton = true;
}

TSharedRef<SWidget> FLootEditorTabFactory_LootProfileWorkflow::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	if (!LootAssetEditor.IsValid()) return SNew(STextBlock).Text(FText::FromString("Error Tab"));
	return SNew(STextBlock).Text(FText::FromString("Loot profile workflow"));
}

FText FLootEditorTabFactory_LootProfileWorkflow::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FWorkflowTabFactory::GetTabToolTipText(Info);
}
