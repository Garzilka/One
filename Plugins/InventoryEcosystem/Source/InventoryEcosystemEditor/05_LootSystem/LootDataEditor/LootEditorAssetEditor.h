#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

/**
 * 
 */
namespace LootEditorAppModes
{
	const FName LootSettings("Loot Settings");
	const FName LootData("Loot Data");
	const FName LootProfile("Loot Profile");
}

namespace LootEditorTabs
{
	const FName LootSettings_List("Loot Settings List");
	const FName LootSettings_Workflow("Loot Settings Workflow");
	
	const FName LootData_List("Loot Data List");
	const FName LootData_Workflow("Loot Data Workflow");
	
	const FName LootProfile_List("Loot Profile List");
	const FName LootProfile_Workflow("Loot Profile Workflow");
}

class ULootData;
class INVENTORYECOSYSTEMEDITOR_API FLootEditorAssetEditor : public FWorkflowCentricApplication, public FNotifyHook, public FEditorUndoClient, public FGCObject
{
	
#pragma region ToolKit

public:

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

#pragma endregion

#pragma region Editor

public:

	void InitLootAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, ULootData* InLootData);

	void BuildToolBar(FToolBarBuilder& ToolbarBuilder);

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override;
	
#pragma endregion

#pragma region TabSpawner

public:

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	void RegisterToolBarTab(const TSharedRef<FTabManager>& InTabManager);

	TSharedRef<SWidget> SpawnTab_Graph();
	TSharedRef<SWidget> SpawnTab_CategoryList();
	
#pragma endregion 

	TObjectPtr<ULootData> GetLootDataRef() { return LootDataRef;}
private:	
	TObjectPtr<ULootData> LootDataRef{ nullptr };
};
