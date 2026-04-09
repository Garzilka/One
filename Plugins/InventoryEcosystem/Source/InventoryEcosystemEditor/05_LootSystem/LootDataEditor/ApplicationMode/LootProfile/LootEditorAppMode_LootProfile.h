#pragma once
#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

/**
 * 
 */
class FLootEditorAssetEditor;
class INVENTORYECOSYSTEMEDITOR_API FLootEditorAppMode_LootProfile : public FApplicationMode
{
public:
	
	FLootEditorAppMode_LootProfile(TSharedPtr<FLootEditorAssetEditor> InLootEditor, FName InApplicationMode, TSharedPtr<FTabManager::FLayout> InLayout);
	virtual auto RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) -> void override;


protected:

	TWeakPtr<FLootEditorAssetEditor> LootEditor;
	FWorkflowAllowedTabSet LootEditorTabFactories;
};