#pragma once

#include "CoreMinimal.h"
#include "InventoryEcosystemEditor/05_LootSystem/LootDataEditor/LootEditorAssetEditor.h"

class SLootObjectBTN : public SCompoundWidget
{
public:
	
	SLATE_BEGIN_ARGS(SLootObjectBTN)
		: _LootObjectGUID(FName("")),
		 _LootAssetEditorRef(TWeakPtr<FLootEditorAssetEditor>())
	{}
		SLATE_ARGUMENT(FName, LootObjectGUID)
		SLATE_ARGUMENT(TWeakPtr<FLootEditorAssetEditor>, LootAssetEditorRef)
		/** Called when the button is clicked */
		SLATE_EVENT( FOnClicked, OnClicked )
		
SLATE_END_ARGS();

	INVENTORYECOSYSTEMEDITOR_API SLootObjectBTN() {};
	INVENTORYECOSYSTEMEDITOR_API FText GetText() const;
	void Construct(const FArguments& InArgs);
protected:
	TWeakPtr<FLootEditorAssetEditor> LootAssetEditor;
	FSlateColor GetColor() const;
	FName LootObjectGUID;
	TSharedPtr<SButton> CategoryBTN;
	FReply Clicked();
	FText LootSpawner_Name;
	/** The delegate to execute when the button is clicked */
	FOnClicked OnClicked;
	
};
