#pragma once
#include "CoreMinimal.h"
#include "InventoryEcosystemEditor/05_LootSystem/LootDataEditor/Factoryes/LootSettingsTabs/LootEditorTabFactory_LootSettingsList.h"

class SLootDataListWidget : public SCompoundWidget
{
public:
	
	SLATE_BEGIN_ARGS(SLootDataListWidget)
		: _CategoryData(FLootCategoryData()),
		 _LootAssetEditorRef(TWeakPtr<FLootEditorAssetEditor>())
	{}
	SLATE_ARGUMENT(FLootCategoryData, CategoryData)
	SLATE_ARGUMENT(TWeakPtr<FLootEditorAssetEditor>, LootAssetEditorRef)
		
SLATE_END_ARGS();

	INVENTORYECOSYSTEMEDITOR_API SLootDataListWidget() {};
	void Construct(const FArguments& InArgs);
protected:
	FLootCategoryData CategoryData;
	TWeakPtr<FLootEditorAssetEditor> LootAssetEditor;
	TSharedPtr<SExpandableArea> CategoryExpand;

	TSharedRef<SWidget> CreateChildren(FLootCategoryData Category, int32 Idx);
	TSharedRef<SWidget> CreateArray(TArray<FName> LootSettings);
	
};
