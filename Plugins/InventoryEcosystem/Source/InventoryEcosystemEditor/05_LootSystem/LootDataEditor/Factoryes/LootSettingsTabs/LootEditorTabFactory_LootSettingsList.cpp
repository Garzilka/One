#include "LootEditorTabFactory_LootSettingsList.h"
#include "EditorStyleSet.h"
#include "InventoryEcosystem/05_LootSystem/Data/LootData.h"
#include "InventoryEcosystemEditor/05_LootSystem/LootDataEditor/LootEditorAssetEditor.h"
#include "InventoryEcosystemEditor/05_LootSystem/LootDataEditor/LootSettings/Widgets/Category/LootDataListWidget.h"
#include "Kismet/KismetStringLibrary.h"

FLootEditorTabFactory_LootSettingsList::FLootEditorTabFactory_LootSettingsList(TSharedRef<FLootEditorAssetEditor> InLootEditor)
	:FWorkflowTabFactory(LootEditorTabs::LootSettings_List, InLootEditor), LootAssetEditor(InLootEditor)
{
	TabLabel = FText::FromString(TEXT("Loot settings list"));
	TabIcon = FSlateIcon(FEditorStyle::GetStyleSetName(), "ContentReference.PickAsset");
	bIsSingleton = true;
}

TSharedRef<SWidget> FLootEditorTabFactory_LootSettingsList::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	if (!LootAssetEditor.IsValid()) return SNew(STextBlock).Text(FText::FromString("Error Tab"));
	
	
	ULootData* LootDataRef = LootAssetEditor.Pin()->GetLootDataRef();
	if (!LootDataRef) return SNew(STextBlock).Text(FText::FromString("LootDataRef Error")); 

	FLootCategoryData CategoryData;
	for (auto LData : LootDataRef->GetLootSettingsData())
	{
		TArray<FName> LootCategoryNames;
		FString NextSplit = LData.Value.Category.ToString();
		FString Key = "|";
		FString Right = "";
		FString Left = "";
		bool HasNext = false;

		if (!UKismetStringLibrary::Split(NextSplit, Key, Left, Right))
		{
			LootCategoryNames.Add(FName(NextSplit));
			TSharedPtr<FLootCategoryData> Category = AddChild(CategoryData.Children, LootCategoryNames);
			if (Category.IsValid())
			{
				Category.Get()->ObjectGUIDS.Add(LData.Key);
			}
			continue;
		}
		
		do 
		{
			HasNext = UKismetStringLibrary::Split(NextSplit, Key, Left, Right);
			if (HasNext && Left.Len() > 0)
			{
				LootCategoryNames.Add(FName(Left));
			}
			
			if (!HasNext && Right.Len() > 0)
			{
				LootCategoryNames.Add(FName(Right));
			}
			NextSplit = Right;
		} while	(HasNext);
		
		TSharedPtr<FLootCategoryData> Category = AddChild(CategoryData.Children, LootCategoryNames);
		if (Category.IsValid())
		{
			Category.Get()->ObjectGUIDS.Add(LData.Key);
		}
	}
	
	return SNew(SBox)
	[
		SNew(SLootDataListWidget)
		.CategoryData(CategoryData)
		.LootAssetEditorRef(LootAssetEditor)
	];
}

FText FLootEditorTabFactory_LootSettingsList::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FWorkflowTabFactory::GetTabToolTipText(Info);
}