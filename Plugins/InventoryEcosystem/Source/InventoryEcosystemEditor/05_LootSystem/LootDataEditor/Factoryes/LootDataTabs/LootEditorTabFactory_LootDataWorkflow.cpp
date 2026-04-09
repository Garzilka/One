#include "LootEditorTabFactory_LootDataWorkflow.h"
#include "EditorStyleSet.h"
#include "InventoryEcosystemEditor/05_LootSystem/LootDataEditor/LootEditorAssetEditor.h"


FLootEditorTabFactory_LootDataWorkflow::FLootEditorTabFactory_LootDataWorkflow(TSharedRef<FLootEditorAssetEditor> InLootEditor)
	:FWorkflowTabFactory(LootEditorTabs::LootData_Workflow, InLootEditor), LootAssetEditor(InLootEditor)
{
	TabLabel = FText::FromString(TEXT("Workflow"));
	TabIcon = FSlateIcon(FEditorStyle::GetStyleSetName(), "ContentReference.PickAsset");
	bIsSingleton = true;
}

TSharedRef<SWidget> FLootEditorTabFactory_LootDataWorkflow::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	if (!LootAssetEditor.IsValid()) return SNew(STextBlock).Text(FText::FromString("Error Tab"));
	return SNew(STextBlock).Text(FText::FromString("Loot data workflow"));
}

FText FLootEditorTabFactory_LootDataWorkflow::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FWorkflowTabFactory::GetTabToolTipText(Info);
}