#pragma once

#include "CoreMinimal.h"
#include "InventoryEcosystemEditor/05_LootSystem/LootDataEditor/LootEditorAssetEditor.h"

class SCategoryBTN : public SCompoundWidget
{
public:
	
	SLATE_BEGIN_ARGS(SCategoryBTN)
		: _StyleSet(&FCoreStyle::Get()),
		 _CategoryName(FName("Empty")),
		 _LootAssetEditorRef(TWeakPtr<FLootEditorAssetEditor>())
	{}
	SLATE_ARGUMENT(const ISlateStyle*, StyleSet)
	SLATE_ARGUMENT(FName, CategoryName)
	SLATE_ARGUMENT(TWeakPtr<FLootEditorAssetEditor>, LootAssetEditorRef)
	/** Called when the button is clicked */
	SLATE_EVENT( FOnClicked, OnClicked )
		
SLATE_END_ARGS();

	INVENTORYECOSYSTEMEDITOR_API SCategoryBTN() {};
	
	INVENTORYECOSYSTEMEDITOR_API FText GetText() const;
	void Construct(const FArguments& InArgs);
	/** See OnClicked event */
	void SetOnClicked(FOnClicked InOnClicked);
protected:
	FName CategoryName;
	FSlateColor GetColor() const;
	TWeakPtr<FLootEditorAssetEditor> LootAssetEditor;
	const ISlateStyle* StyleSet;
	/** A reference to the expander button */
	TSharedPtr<SButton> CategoryBTN;
	
	FMargin GetExpanderPadding() const;
	EVisibility GetExpanderVisibility() const;
	FReply Clicked();
	
	const FSlateBrush* GetImage() const;

	/** The delegate to execute when the button is clicked */
	FOnClicked OnClicked;
};
