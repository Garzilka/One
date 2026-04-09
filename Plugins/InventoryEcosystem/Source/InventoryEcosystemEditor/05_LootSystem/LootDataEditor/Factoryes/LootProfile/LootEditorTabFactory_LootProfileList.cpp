#include "LootEditorTabFactory_LootProfileList.h"
#include "EditorStyleSet.h"
#include "InventoryEcosystemEditor/05_LootSystem/LootDataEditor/LootEditorAssetEditor.h"

FLootEditorTabFactory_LootProfileList::FLootEditorTabFactory_LootProfileList(TSharedRef<FLootEditorAssetEditor> InLootEditor)
	:FWorkflowTabFactory(LootEditorTabs::LootProfile_List, InLootEditor), LootAssetEditor(InLootEditor)
{
	TabLabel = FText::FromString(TEXT("Inventory Tool"));
	TabIcon = FSlateIcon(FEditorStyle::GetStyleSetName(), "ContentReference.PickAsset");
	bIsSingleton = true;
}

TSharedRef<SWidget> FLootEditorTabFactory_LootProfileList::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	if (!LootAssetEditor.IsValid()) return SNew(STextBlock).Text(FText::FromString("Error Tab"));
	return SNew(STextBlock).Text(FText::FromString("Inventory Tool"));
}

FText FLootEditorTabFactory_LootProfileList::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FWorkflowTabFactory::GetTabToolTipText(Info);
}