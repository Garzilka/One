#pragma once
#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

/**
 * 
 */
class FLootEditorAssetEditor;
class INVENTORYECOSYSTEMEDITOR_API FLootEditorAppMode_LootData : public FApplicationMode
{
public:
	
	FLootEditorAppMode_LootData(TSharedPtr<FLootEditorAssetEditor> InLootEditor, FName InApplicationMode, TSharedPtr<FTabManager::FLayout> InLayout);
	virtual auto RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) -> void override;


protected:

	TWeakPtr<FLootEditorAssetEditor> LootEditor;
	FWorkflowAllowedTabSet LootEditorTabFactories;
};
