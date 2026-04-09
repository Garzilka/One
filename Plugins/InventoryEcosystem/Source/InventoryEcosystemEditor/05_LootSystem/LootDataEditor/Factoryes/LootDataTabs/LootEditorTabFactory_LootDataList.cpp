#include "LootEditorTabFactory_LootDataList.h"
#include "EditorStyleSet.h"
#include "InventoryEcosystemEditor/05_LootSystem/LootDataEditor/LootEditorAssetEditor.h"


FLootEditorTabFactory_LootDataList::FLootEditorTabFactory_LootDataList(TSharedRef<FLootEditorAssetEditor> InLootEditor)
	:FWorkflowTabFactory(LootEditorTabs::LootData_List, InLootEditor), LootAssetEditor(InLootEditor)
{
	TabLabel = FText::FromString(TEXT("Loot data list"));
	TabIcon = FSlateIcon(FEditorStyle::GetStyleSetName(), "ContentReference.PickAsset");
	bIsSingleton = true;
}

TSharedRef<SWidget> FLootEditorTabFactory_LootDataList::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	if (!LootAssetEditor.IsValid()) return SNew(STextBlock).Text(FText::FromString("Error Tab"));
	
	return SNew(STextBlock).Text(FText::FromString("Loot data list"));
}

FText FLootEditorTabFactory_LootDataList::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FWorkflowTabFactory::GetTabToolTipText(Info);
}